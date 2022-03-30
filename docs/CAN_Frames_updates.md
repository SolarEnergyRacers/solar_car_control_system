# BMS
| Frame Content                             | Default Interval                   | Car State Interval | defintions.h            |
|-------------------------------------------|------------------------------------|--------------------|-------------------------|
| BMU HEARTBEAT AND SERIAL NUMBER           | 1 Hz                               |                    | MAXAGE_BMU_HEARTBEAT    |
| CMU Number, PCB Temp, Cell Temp (per CMU) | 1 Hz                               |                    | MAXAGE_CMU_TEMP         |
| Cell Voltages 1 (per CMU)                 | 1 Hz                               |                    | MAXAGE_CMU_VOLTAGES     |
| Cell Voltages 2 (per CMU)                 | 1 Hz                               |                    | MAXAGE_CMU_VOLTAGES     |
| Pack SoC                                  | 1 Hz                               |                    | MAXAGE_PACK_SOC         |
| Balance SoC                               | 1 Hz                               |                    | MAXAGE_BALANCE_SOC      |
| Charger Control Info (not relevant)       | 10 Hz                              | 0                  | MAXAGE_CHARGER_CONTROL  |
| Precharge Status                          | 1 Hz and on precharge state change |                    | MAXAGE_PRECHARGE_STATUS |
| min/max cell voltage                      | 10 Hz                              | 1 Hz               | MAXAGE_MIN_MAX_U_CELL   |
| min/max cell temp                         | 1 Hz                               |                    | MAXAGE_MIN_MAX_T_CELL   |
| Pack Voltage & Current                    | 10 Hz                              | 2 Hz               | MAXAGE_PACK_VOLTAGE     |
| Pack Status (& error flags)               | 1 Hz                               |                    | MAXAGE_PACK_STATUS      |
| Pack Fan status                           | 1 Hz                               |                    | MAXAGE_PACK_FAN_STATUS  |
| Extended Pack status (& error flags)      | 1 Hz                               |                    | MAXAGE_EXT_PACK_STATUS  |

# MPPT (x3)
| Frame Content                            | Default Interval | Car State Interval | defintions.h           | 
|------------------------------------------|------------------|--------------------|------------------------|
| Input Voltage & Current                  | 1 Hz             |                    | MAXAGE_MPPT_INPUT      |
| Output Voltage & Current                 | 1 Hz             |                    | MAXAGE_MPPT_OUTPUT     |
| Temperatures                             | 1 Hz             |                    | MAXAGE_MPPT_TEMP       |
| Aux. Power Voltages                      | 1 Hz             |                    | MAXAGE_MPPT_AUX_POWER  |
| Limits (Uout & Iin)                      | 1 Hz             | 0.1 Hz             | MAXAGE_MPPT_LIMITS     |
| Status (& error flags)                   | 1 Hz             |                    | MAXAGE_MPPT_STATUS     |
| Power Connector (Uout & Connector Temp.) | 1 Hz             |                    | MAXAGE_MPPT_POWER_CONN |
