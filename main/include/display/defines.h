#define WROOM
#define MAX
//#define DEBUG
//#define DO_NOT_DISPLAY

#ifdef WROOM
    /// @brief Pin MOSI dla WROOM.
    #ifndef MOSI
        #define MOSI 23
    #endif
    /// @brief Pin CS dla WROOM.
    #ifndef CS
        #define CS 5
    #endif
    /// @brief Pin CLK dla WROOM.
    #ifndef CLK
        #define CLK 18
    #endif
#endif

#ifdef MINI
    /// @brief Pin MOSI dla MINI.
    #ifndef MOSI
        #define MOSI 6
    #endif
    /// @brief Pin CS dla MINI.
    #ifndef CS
        #define CS 7
    #endif
    /// @brief Pin CLK dla MINI.
    #ifndef CLK
        #define CLK 4
    #endif
#endif

#ifdef MAX
    #define MAX_COUNT 8
    #define MAX_DATA_SIZE_BYTES MAX_COUNT*16
    #define ROWS 8
    #define REPEAT 8
    #define SCROLL_TIME 100
#endif

#define CONDITION_CHECK_INTERVAL_MS 2

#define BLOCK_UNTIL(action, bool_condition)  while(!(bool_condition)){vTaskDelay(pdMS_TO_TICKS(CONDITION_CHECK_INTERVAL_MS));};    \
                                            bool_condition = !bool_condition;   \
                                            action;                             \
                                            bool_condition = !bool_condition  