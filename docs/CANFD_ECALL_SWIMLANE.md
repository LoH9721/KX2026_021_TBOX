# CAN FD eCall 样机流程

## 接口边界

- 车辆侧 CAN1 接口当前由 HC32 CAN2 外设承载，PE2/TX、PE3/RX，标准 ID，名义段 500 kbit/s，数据段 2 Mbit/s；收发器使能/待机脚仍需按实板确认。
- 样机输入 `0x126`：CAN FD+BRS、DLC 8；Byte0 bit0 为碰撞，bit1 为翻滚，Byte1/2 为纵向/横向 Delta-V 原始值，Byte3 为乘员数。
- DBC 输出 `0x21C TBOX_EcallInfo`：CAN FD+BRS、DLC 8、周期 100 ms。
- `0x126` 不是所提供 DBC 的正式报文，只是当前样机测试契约；IMU、NM、CanTp 和 UDS 均不接入。

## 泳道图

```mermaid
sequenceDiagram
    participant Tool as CAN FD分析仪/车辆侧
    participant ISR as CAN1中断
    participant CanTask as CanTask 10 ms
    participant EcallTask as EcallTask
    participant Time as TimeService
    participant Gnss as GNSS服务
    participant Ecall as Ecall状态机
    participant Modem as Modem/平台

    Tool->>ISR: 0x126，FD+BRS，8 Byte
    ISR->>ISR: 读取硬件FIFO并写固定Ring Buffer
    Note over ISR: 不调用OS、不打印日志、不处理业务
    CanTask->>CanTask: 取帧并校验ID/FD/BRS/DLC
    CanTask->>CanTask: 缓存最新字段（有效2秒）
    alt crash 0->1且已重新武装
        CanTask->>EcallTask: ECALL_TASK_EVT_CAN_TRIGGER
        CanTask->>CanTask: 冻结本次触发字段直至MSD取走
        EcallTask->>EcallTask: 记录最早触发tick
        EcallTask->>Time: 查询GNSS优先、CCLK兜底的UTC
        Time-->>EcallTask: 当前UTC或INVALID
        EcallTask->>Ecall: 自动触发直接进入WAIT_GNSS
        EcallTask->>Gnss: 请求/读取当前点与T-1…T-9
        Gnss-->>EcallTask: 有效坐标或无效零坐标
        EcallTask->>Time: UTC此前无效时再次查询并回算触发时间
        EcallTask->>CanTask: 读取新鲜事故字段
        EcallTask->>Ecall: 冻结MSD快照
        Ecall->>Modem: 请求独立数据和语音流程
        Note over EcallTask,Modem: MSD[107..110]=纵向DV/横向DV/乘员数/翻滚
    else crash保持1
        CanTask->>CanTask: 不重复触发
    else crash=0
        CanTask->>CanTask: 重新武装下一次上升沿
    end

    loop 每100 ms
        CanTask->>Tool: 0x21C eCall状态与通话时长
    end
```

CAN触发属于自动触发，不进入10 s手动取消窗口。MSD触发时间取 `0x126` 上升沿进入任务系统时的时间；后续等待GNSS、TCP重连和数据重试都不改变该值。

## 实板验证顺序

1. 先只观察 `0x21C` 是否每 100 ms 发出，确认标准 ID、FD、BRS、DLC 8 和 500K/2M。
2. 发送 `0x126: 00 00 00 FF 00 00 00 00` 只完成触发重新武装，不会启动 eCall。
3. 只有在平台、号码和真实外呼时机均已获明确授权，或已切换到不会真实拨号的测试环境后，才发送 `0x126: 01 12 08 02 00 00 00 00`；预期只触发一次 eCall。
4. 核对上传 MSD 的 111 字节负载中 Byte107–110 为 `12 08 02 00`。
5. 连续保持 crash=1 不应重复触发；先发 crash=0 后再次置 1 才允许下一次触发。

> 收发器 EN/STB、终端电阻和分析仪接线必须按原理图确认；当前代码没有为未知硬件脚位添加 GPIO 控制。
