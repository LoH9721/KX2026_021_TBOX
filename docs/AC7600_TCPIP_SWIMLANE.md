# AC7600 网络、MSD、语音与回呼流程泳道图

## 适用范围与证据边界

本文描述当前源码中的 AC7600 主路径，包括上电自检、PDP 保持、TCP 短连接、每会话注册、MSD 关联应答、数据/语音重试、通话结束后的平台回呼等待，以及异常链路恢复。

- 当前策略：保持 PDP，按业务建立 TCP；每个 TCP 会话都重新注册，业务结束后只执行 `AT+CIPCLOSE=0`，不执行 `AT+NETCLOSE`。
- 当前短连接模式不发送应用层 heartbeat；旧长连接的 `heartbeat_ack` 日志只作为历史实板证据。
- 数据成功以完整关联的 `msd_ack` 为准；`+CIPSEND` 仅表示数据已交给模组 TCP 栈。
- 语音不等待 `msd_ack`，但必须等待当前完整 AT 事务结束后才能使用同一 UART4。
- 当前代码已实现 60 min 数据/语音尝试窗口和通话后的 60 min 回呼等待；本轮改动尚未编译、烧写或实板验证。
- 原厂依据：`A76XX Series_TCPIP_Application Note_V1.05.pdf`、`A76XX_Series_AT_Command_Manual_V2.02.pdf`。

## 1. 总体业务流程

```mermaid
flowchart TD
    A[KL15 ON / TBOX上电] --> B[AT同步、SIM检查、LTE注册]
    B --> C[NETOPEN并保留PDP]
    C --> D[CIPOPEN建立启动自检TCP]
    D --> E[发送注册报文并等待register_ack]
    E -->|成功| F[延时500 ms后CIPCLOSE]
    E -->|失败或5 s超时| G[关闭TCP并记录失败]
    F --> H[PDP保持，TCP关闭，后台GNSS/时间运行]
    G --> H
    H --> I{收到有效eCall触发?}
    I -->|否| H
    I -->|是| J[冻结触发、时间、GNSS/CAN快照和ser]
    J --> K[数据路径：重建TCP、注册、上传MSD]
    J --> L[语音路径：注册网络可用后独立ATD]
    K --> M{完整关联msd_ack?}
    M -->|是| N[数据成功，500 ms后关闭TCP]
    M -->|否/15 s超时| O[关闭TCP，30 s/120 s后重试]
    O -->|60 min内| K
    O -->|窗口到期| P[数据尝试结束]
    L --> Q[拨号/接通/释放原因判断]
    Q -->|异常且60 min内| R[30 s首次，之后120 s重试]
    R --> L
    Q -->|正常释放| S[进入平台回呼等待]
    S -->|RING/CRING/来电CLCC| T[ATA接听平台回拨]
    T --> Q
    S -->|60 min到期且无通话| U[结束本次eCall会话]
```

## 2. 上电自检、PDP与短连接注册

```mermaid
sequenceDiagram
    participant MT as ModemTask
    participant MS as ModemService
    participant MN as ModemNetwork
    participant MP as ModemPlatform
    participant Port as ModemPort/UART4
    participant AC as AC7600
    participant V as AECS平台

    MT->>MS: KL15 ON，周期调度
    MS->>MN: 请求网络在线
    MN->>Port: AT / ATE0 / AT+CPIN? / AT+CSQ / AT+CEREG?
    Port->>AC: 串行发送AT命令
    AC-->>Port: SIM READY，CEREG=1或5
    MN->>Port: AT+NETOPEN? / 必要时NETCLOSE / NETOPEN / IPADDR
    AC-->>Port: +NETOPEN: 0，IP地址
    MN->>Port: CTCPKA / CIPCFG / CIPRXGET=1 / CIPOPEN
    AC->>V: 建立TCP
    AC-->>Port: +CIPOPEN: 0,0
    MN-->>MS: TCP_CONNECTED
    MS->>MP: 开始本会话注册
    MP->>Port: CIPSEND注册帧
    Port->>AC: 01 + 长度 + {role,deviceId}
    AC->>V: 注册数据
    V-->>AC: register_ack
    AC-->>Port: +CIPRXGET通知
    MP->>Port: CIPRXGET=4查询长度，CIPRXGET=3读取数据
    MP->>MP: 校验帧头、result=ok和deviceId
    alt 5 s内注册成功
        MP-->>MS: REGISTERED
        MS->>MS: 安排500 ms关闭延时
        MS->>MN: RequestTcpClose
        MN->>Port: AT+CIPCLOSE=0
        AC-->>Port: +CIPCLOSE: 0,0
        MN-->>MS: TCP_CLOSED_PDP_RETAINED
    else 负应答、字段不匹配或超时
        MP-->>MS: PLATFORM_FAILURE
        MS->>MN: 关闭当前TCP
    end
```

说明：`OK` 只代表命令已受理；`NETOPEN`、`CIPOPEN`、`CIPSEND` 和 `CIPCLOSE` 都必须分别等待对应异步结果。短连接关闭后 PDP 保留，下一次事件直接从 `CIPOPEN` 重建会话。

## 3. 手动/自动触发、取消窗口与快照

```mermaid
sequenceDiagram
    participant SRC as SOS/CAN/自动触发源
    participant ET as EcallTask
    participant TS as TimeService
    participant GS as GnssService
    participant CS as CanService
    participant EC as Ecall状态机
    participant MS as ModemService

    SRC->>ET: 请求触发，记录最早系统tick
    ET->>TS: 获取当前UTC
    alt UTC已经有效
        TS-->>ET: GNSS UTC或网络UTC
        ET->>ET: 冻结触发UTC
    else UTC尚未有效
        TS-->>ET: INVALID
        ET->>ET: 保留触发tick，稍后回算
    end
    ET->>EC: 合并本批触发mask
    alt 仅手动SOS
        EC->>EC: 进入10 s取消窗口
        alt 用户取消
            EC-->>ET: 会话结束，不上传、不拨号
        else 10 s到期
            EC->>EC: WAIT_GNSS
        end
    else 自动/CAN触发
        EC->>EC: 直接WAIT_GNSS
    end
    ET->>GS: 最多等待3 s后读取当前点和T-1…T-9
    ET->>CS: 读取新鲜事故字段
    ET->>TS: 若触发UTC仍无效，再取当前UTC
    TS-->>ET: 当前UTC或INVALID
    ET->>ET: 当前UTC-触发后经过秒数，回算触发UTC；仍无效则填0
    ET->>EC: 冻结完整MSD快照
    EC->>MS: 请求紧急数据和语音流程
```

同一事件的重连和重发复用已冻结 MSD、`ser` 和触发时间，不用重发时刻覆盖原始触发时刻。

## 4. MSD短连接上传与关联应答

```mermaid
sequenceDiagram
    participant ET as EcallTask
    participant MS as ModemService
    participant MN as ModemNetwork
    participant MP as ModemPlatform
    participant Port as ModemPort
    participant AC as AC7600
    participant V as AECS平台

    ET->>MS: RequestEmergencyCall(冻结MSD)
    MS->>MP: 保存同一事件帧和ser
    MS->>MN: PDP保留状态下请求TCP在线
    MN->>Port: CIPOPEN
    AC->>V: TCP连接
    AC-->>Port: +CIPOPEN: 0,0
    MS->>MP: 每个新TCP会话重新注册
    MP->>V: register
    V-->>MP: 合法register_ack
    MP->>Port: CIPSEND动态长度MSD V0.2帧
    Port->>AC: 0x10 + 4字节长度 + 111字节标准区 + TLV扩展
    AC-->>Port: +CIPSEND: link,req,cnf
    alt req等于cnf
        MP->>MP: 开始15 s应用ACK计时
        V-->>MP: msd_ack
        MP->>MP: 校验type/result/deviceId/ser/identifier/triggerTimeUtc
        alt 六项完整且匹配
            MP-->>MS: MSD_ACK_CONFIRMED
            MS->>MN: 500 ms后CIPCLOSE
        else 缺字段、错ser、负应答或旧ACK
            MP->>MP: 不确认，继续等待有效ACK
        end
    else CIPSEND失败
        MP->>MP: 本次数据尝试失败
    end
    alt 15 s未取得合法ACK
        MP-->>MS: MSD_ACK_TIMEOUT_CLOSE
        MS->>MN: CIPCLOSE
        MP->>MP: 首次等待30 s，后续等待120 s
        Note over MP: 60 min总窗口不因断线、注册或语音而重置
    end
```

## 5. 数据与语音的AT口仲裁

```mermaid
sequenceDiagram
    participant MP as ModemPlatform
    participant MC as ModemCall
    participant Port as ModemPort
    participant AC as AC7600

    MP->>Port: CIPSEND命令
    AC-->>Port: >
    MP->>Port: 二进制负载
    AC-->>Port: +CIPSEND结果
    Port-->>MP: 当前事务完成并由Platform消费
    Note over MP,MC: 只有到这里才可切换AT所有者
    MC->>Port: 音频准备AT
    Port-->>MC: 完成
    MC->>Port: ATD号码;
    AC-->>Port: OK / CLCC / VOICE CALL URC
```

语音业务不以 `msd_ack` 为门槛；如果数据事务正在 `>` 提示符、负载发送或结果等待阶段，语音只等待该事务结束，不会插入其中。TCP关闭不等于语音挂断，代码不因 `CIPCLOSE` 主动发送 `ATH`。

## 6. 外呼、释放原因与平台回拨

```mermaid
sequenceDiagram
    participant EC as Ecall状态机
    participant MC as ModemCall
    participant Port as ModemPort
    participant AC as AC7600
    participant PSAP as 平台/接警中心

    EC->>MC: 请求独立语音
    MC->>Port: 音频路径准备
    MC->>Port: ATD<号码>;
    AC->>PSAP: 外呼
    AC-->>Port: CLCC拨号/振铃/接通状态
    AC-->>Port: VOICE CALL: BEGIN
    Port-->>MC: CONNECTED
    MC-->>EC: CALL_ACTIVE
    PSAP-->>AC: 挂断
    AC-->>Port: VOICE CALL: END / NO CARRIER
    MC->>Port: AT+CEER
    alt 正常释放
        MC-->>EC: CALL_ENDED
        EC->>EC: CALLBACK_STANDBY，启动60 min
        Note over EC,AC: 保持模组可注册入网并等待平台回拨
        PSAP->>AC: 平台回拨
        AC-->>Port: RING / +CRING / 来电CLCC
        Port-->>MC: INCOMING
        MC->>Port: 音频准备后ATA
        AC-->>Port: OK / 接通URC
        MC-->>EC: CALL_ACTIVE
    else 异常释放且仍在60 min语音窗口
        MC->>MC: 首次30 s、后续120 s等待
        MC->>Port: 再次ATD
    end
    alt 回呼等待满60 min且当前无通话
        EC->>MC: 退出CALLBACK_STANDBY
        MC-->>EC: IDLE
        EC->>EC: COMPLETE后回到STANDBY
    end
```

平台来电通过 `RING`、`+CRING:` 或 `+CLCC` 来电方向/状态识别。`ATA`失败且尚未接通时返回回呼等待，不把一次接听失败误转为主动外呼重试。

## 7. TCP/PDP异常恢复

```mermaid
flowchart TD
    A[收到IPCLOSE或CIPEVENT] --> B{语音AT事务进行中?}
    B -->|是| C[记录传输丢失，等待语音终态]
    B -->|否| D[ModemNetwork重新AT同步]
    C --> D
    D --> E{PDP仍有效?}
    E -->|是| F[重新CIPOPEN]
    E -->|否或PDN DEACT| G[NETCLOSE清理后NETOPEN]
    G --> F
    F --> H[每个新TCP会话重新注册]
    H --> I{有待确认MSD?}
    I -->|是| J[重发同一冻结帧和ser]
    I -->|否| K[按短连接策略关闭TCP]
```

## 8. 当前验证状态

| 流程 | 状态 | 说明 |
|---|---|---|
| UART、SIM、LTE、PDP、旧长连接注册/心跳/MSD | 有历史实板证据 | 不能替代当前短连接V0.2回归。 |
| PDP保持、TCP短连接、每会话注册 | 已实现并有软件验证 | 待实板验证 `CIPCLOSE` 后不执行 `NETCLOSE`。 |
| V0.2动态MSD、严格关联ACK、拆包/粘包 | 已实现并有软件验证 | 后台需按V0.2生成完整ACK。 |
| 数据/语音独立和30 s/120 s/60 min重试 | 已实现并有软件验证 | 待受控实板验证。 |
| 60 min回呼等待、来电识别和ATA | 已实现、未编译 | 待编译和平台真实回拨验证。 |
| UTC低频校时和MSD触发时间冻结 | 已实现、未编译 | 详见 `AC7600_GNSS_SWIMLANE.md`。 |
