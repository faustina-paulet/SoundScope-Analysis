/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  FREQ_PANEL                       1       /* callback function: OnFrequencyPanelCB */
#define  FREQ_PANEL_IDC_SWITCH_PANEL      2       /* control type: binary, callback function: OnSwitchPanelCB */
#define  FREQ_PANEL_FREQ_BUTTON           3       /* control type: command, callback function: OnFrequencyButtonCB */
#define  FREQ_PANEL_IDC_NR_PUNCTE         4       /* control type: ring, callback function: (none) */
#define  FREQ_PANEL_FREQ_PEAK             5       /* control type: numeric, callback function: (none) */
#define  FREQ_PANEL_POWER_PEAK            6       /* control type: numeric, callback function: (none) */
#define  FREQ_PANEL_GRAPH_SPECTRU         7       /* control type: graph, callback function: (none) */
#define  FREQ_PANEL_GRAPH_RAW_DATA        8       /* control type: graph, callback function: (none) */
#define  FREQ_PANEL_GRAPH_WINDOW          9       /* control type: graph, callback function: (none) */
#define  FREQ_PANEL_GRAPH_RAW_WINDOW      10      /* control type: graph, callback function: (none) */
#define  FREQ_PANEL_GRAPH_FILTER          11      /* control type: graph, callback function: (none) */
#define  FREQ_PANEL_GRAPH_SPECTRUM_FILTER 12      /* control type: graph, callback function: (none) */
#define  FREQ_PANEL_FILTER_BUTTON         13      /* control type: command, callback function: OnFilterButtonCB */
#define  FREQ_PANEL_WINDOW_TYPE           14      /* control type: ring, callback function: (none) */
#define  FREQ_PANEL_FILTER_TYPE           15      /* control type: ring, callback function: (none) */
#define  FREQ_PANEL_IDC_CUT_FREQ          16      /* control type: numeric, callback function: (none) */
#define  FREQ_PANEL_IDC_ORDER             17      /* control type: numeric, callback function: (none) */
#define  FREQ_PANEL_IDC_FPASS             18      /* control type: numeric, callback function: (none) */
#define  FREQ_PANEL_IDC_SECUNDA           19      /* control type: ring, callback function: (none) */

#define  PANEL                            2       /* callback function: ON_WAVE_PANEL */
#define  PANEL_GRAPH_RAW_DATA             2       /* control type: graph, callback function: OnLoadButtonCB */
#define  PANEL_GRAPH_FILTERED_DATA        3       /* control type: graph, callback function: (none) */
#define  PANEL_PREV                       4       /* control type: command, callback function: On_Prev */
#define  PANEL_START                      5       /* control type: numeric, callback function: (none) */
#define  PANEL_Stop                       6       /* control type: textMsg, callback function: (none) */
#define  PANEL_DECORATION                 7       /* control type: deco, callback function: (none) */
#define  PANEL_STOP                       8       /* control type: numeric, callback function: (none) */
#define  PANEL_NEXT                       9       /* control type: command, callback function: On_Next */
#define  PANEL_Start                      10      /* control type: textMsg, callback function: (none) */
#define  PANEL_NUMERIC_MIN                11      /* control type: numeric, callback function: (none) */
#define  PANEL_Interval                   12      /* control type: textMsg, callback function: (none) */
#define  PANEL_FILTRE                     13      /* control type: ring, callback function: FiltreType */
#define  PANEL_TEXTMSG_4                  14      /* control type: textMsg, callback function: (none) */
#define  PANEL_DIM_WINDOW                 15      /* control type: numeric, callback function: (none) */
#define  PANEL_OnAplicaButton             16      /* control type: command, callback function: On_Aplica */
#define  PANEL_Load_wave                  17      /* control type: command, callback function: OnLoadButtonCB */
#define  PANEL_TEXTMSG_5                  18      /* control type: textMsg, callback function: (none) */
#define  PANEL_NUMERIC_MEAN               19      /* control type: numeric, callback function: (none) */
#define  PANEL_IDC_SAVE_BUTTON            20      /* control type: command, callback function: OnSaveCB */
#define  PANEL_NUMERIC_MAX                21      /* control type: numeric, callback function: (none) */
#define  PANEL_NUMERIC_MIN_INDEX          22      /* control type: numeric, callback function: (none) */
#define  PANEL_NUMERIC_MAX_INDEX          23      /* control type: numeric, callback function: (none) */
#define  PANEL_NUMERIC_DISPERSIE          24      /* control type: numeric, callback function: (none) */
#define  PANEL_NUMERIC_MEDIANA            25      /* control type: numeric, callback function: (none) */
#define  PANEL_NR_ESAN                    26      /* control type: numeric, callback function: (none) */
#define  PANEL_FRECV_ES                   27      /* control type: numeric, callback function: (none) */
#define  PANEL_ALPHA                      28      /* control type: numeric, callback function: (none) */
#define  PANEL_GRAPH_HISTOGRAMA           29      /* control type: graph, callback function: (none) */
#define  PANEL_NUM_ZERO                   30      /* control type: numeric, callback function: (none) */
#define  PANEL_ANVELOPA                   31      /* control type: command, callback function: Generare_Anvelopa */
#define  PANEL_DERIVATA                   32      /* control type: command, callback function: Generate_Deriv */
#define  PANEL_IDC_SWITCH_PANEL           33      /* control type: binary, callback function: OnSwitchPanelCB */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK FiltreType(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Generare_Anvelopa(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Generate_Deriv(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK On_Aplica(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK On_Next(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK On_Prev(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ON_WAVE_PANEL(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnFilterButtonCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnFrequencyButtonCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnFrequencyPanelCB(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnLoadButtonCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnSaveCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnSwitchPanelCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
