/*******************************************************
  * Name    :RteSys.c
  * Author  :WangHu
  * Function:****
  * Version :V1.0.0
  * Data    :2023.12.12
*******************************************************/
/*-------------include file---------------------------*/
#include "Types.h"
#include "RteSys.h"
/*-------------variable statement---------------------*/
static RteSwitchState_t s_aeRteSysSwitchState[RTESYS_SWITCH_NUM];
static RteSwitchState_t s_aeRteSysOutputState[RTESYS_OUTPUT_NUM];

/*******************************************************
  * Name    :RteSys_SetSwitchState
  * Author  :LoH
  * Function:Set one normalized TBOX switch state.
  * Version :V1.0.0
  * Data    :2026.09.06
*******************************************************/
void RteSys_SetSwitchState(RteSysSwitchId_t enSwitch, RteSwitchState_t enState)
{
    if (enSwitch < RTESYS_SWITCH_NUM)
    {
        s_aeRteSysSwitchState[enSwitch] = enState;
    }
}

/*******************************************************
  * Name    :RteSys_GetSwitchState
  * Author  :LoH
  * Function:Get one normalized TBOX switch state.
  * Version :V1.0.0
  * Data    :2026.09.06
*******************************************************/
RteSwitchState_t RteSys_GetSwitchState(RteSysSwitchId_t enSwitch)
{
    if (enSwitch < RTESYS_SWITCH_NUM)
    {
        return s_aeRteSysSwitchState[enSwitch];
    }
    return RTE_SWITCH_INACTIVE;
}

/*******************************************************
  * Name    :RteSys_SetOutputState
  * Author  :LoH
  * Function:Set one normalized TBOX output request.
  * Version :V1.0.0
  * Data    :2026.09.06
*******************************************************/
void RteSys_SetOutputState(RteSysOutputId_t enOutput, RteSwitchState_t enState)
{
    if (enOutput < RTESYS_OUTPUT_NUM)
    {
        s_aeRteSysOutputState[enOutput] = enState;
    }
}

/*******************************************************
  * Name    :RteSys_GetOutputState
  * Author  :LoH
  * Function:Get one normalized TBOX output request.
  * Version :V1.0.0
  * Data    :2026.09.06
*******************************************************/
RteSwitchState_t RteSys_GetOutputState(RteSysOutputId_t enOutput)
{
    if (enOutput < RTESYS_OUTPUT_NUM)
    {
        return s_aeRteSysOutputState[enOutput];
    }
    return RTE_SWITCH_INACTIVE;
}
