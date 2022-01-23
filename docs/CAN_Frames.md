# BMS
| Frame Content                             | Default Interval                   | Car State Interval |
|-------------------------------------------|------------------------------------|--------------------|
| BMU HEARTBEAT AND SERIAL NUMBER           | 1 Hz                               |                    |
| CMU Number, PCB Temp, Cell Temp (per CMU) | 1 Hz                               |                    |
| Cell Voltages 1 (per CMU)                 | 1 Hz                               |                    |
| Cell Voltages 2 (per CMU)                 | 1 Hz                               |                    |
| Pack SoC                                  | 1 Hz                               |                    |
| Balance SoC                               | 1 Hz                               |                    |
| Charger Control Info (not relevant)       | 10 Hz                              |                    |
| Precharge Status                          | 1 Hz and on precharge state change |                    |
| min/max cell voltage                      | 10 Hz                              |                    |
| min/max cell temp                         | 1 Hz                               |                    |
| Pack Voltage & Current                    | 10 Hz                              |                    |
| Pack Status (& error flags)               | 1 Hz                               |                    |
| Pack Fan status                           | 1 Hz                               |                    |
| Extended Pack status (& error flags)      | 1 Hz                               |                    |

# MPPT (x3)
| Frame Content                            | Default Interval | Car State Interval |
|------------------------------------------|------------------|--------------------|
| Input Voltage & Current                  | 1 Hz             |                    |
| Output Voltage & Current                 | 1 Hz             |                    |
| Temperatures                             | 1 Hz             |                    |
| Aux. Power Voltages                      | 1 Hz             |                    |
| Limits (Uout & Iin)                      | 1 Hz             |                    |
| Status (& error flags)                   | 1 Hz             |                    |
| Power Connector (Uout & Connector Temp.) | 1 Hz             |                    |
