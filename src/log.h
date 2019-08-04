// Last Update:2019-08-03 18:17:24


#ifndef LOG_H
#define LOG_H

#define BASIC() printf("%s:%d(%s) ", __FILE__, __LINE__, __FUNCTION__)
#define LOGI(args...) BASIC();printf(args)
#define LOGE(args...) LOGI(args)

#endif  /*LOG_H*/
