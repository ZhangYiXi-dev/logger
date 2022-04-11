       # logger
日志器
1.整体结构
![image](https://img-blog.csdnimg.cn/img_convert/338da669525066a4df47ca203a5d3a9c.png)

2.调用方法

```cpp
#include <iostream>
#include "log.h"

zyx::Logger::ptr log_1=(new zyx::LoggerManager(zyx::LogLevel::Level::DEBUG, true, true))->Getlogger(); //默认写日志方式
zyx::Logger::ptr log_2=(new zyx::LoggerManager(zyx::LogLevel::Level::DEBUG, true, {"zidingyi.txt"},"%D 行号:%l %m%n"))->Getlogger();//自定义方式

int main()
{
    ZYX_LOG_DEBUG(log_1,"addr erro");
    ZYX_LOG_INFO(log_2, "ip erro");      
}
```

```cpp
// 配置一：该方式为默认方式，默认向控制台，test.txt文件按格式"%D %r-%p-%t %m%n"输出
  //参数一：日志器级别 可选： 
        /// DEBUG 级别
        DEBUG = 1,
        /// INFO 级别
        INFO = 2,
        /// WARN 级别
        WARN = 3,
        /// ERROR 级别
        ERROR = 4,
        /// FATAL 级别
        FATAL = 5
  //参数二：是否向控制台输出，true为输出
  //参数三：是否向指定默认文件输出，true为输出
  zyx::Logger::ptr log_1=(new zyx::LoggerManager(zyx::LogLevel::Level::DEBUG, true, true))->Getlogger();
  
 // 配置二：该方式为自定义输出方式
  //参数一、二同上
  //参数三：向指定文件（可以多个）中输出
  //参数四：自定义输出格式
            %p:输出事件level
            %r:输出运行事件
            %m:输出自己加入的信息（如IP error）
            %t:输出线程号
            %n：换行
            %f:输出文件名
            %l:输出行号
            %N：输出线程名称
            %T:输出tab
            %F:输出协程号
            %D:输出日期
  zyx::Logger::ptr log_2=(new zyx::LoggerManager(zyx::LogLevel::Level::DEBUG, true, {"zidingyi.txt","zidingyi2.txt"},"%D 行号:%l %m%n"))->Getlogger();//自定义方式
  
  //注意，要输出"addr erro"，"ip erro"格式里要加%m
  //可选方式有 ZYX_LOG_DEBUG(),ZYX_LOG_LEVEL_WARN(),ZYX_LOG_INFO(),ZYX_LOG_LEVEL_ERROR(),ZYX_LOG_LEVEL_FATAL()
   ZYX_LOG_DEBUG(log_1,"addr erro");
   ZYX_LOG_INFO(log_2, "ip erro");
  
  //ZYX_LOG_DEBUG ZYX_LOG_INFO为宏定义(文件中已经定义好，不需要再定义)
```
 
 // 下述是写代码时遇到一些问题的记录：
  //例：注意宏定义中的开头{}，因为宏定义会在定义处直接展开，如果不括起来，会出现重复定义eptr的错误

```cpp
  #define ZYX_LOG_LEVEL_DEBUG(logger, level,str) \
        {\
        zyx::LogEvent::ptr eptr(new zyx::LogEvent(logger, level, \
                        __FILE__, __LINE__, 0, 0,\
                0, time(0), "root"));\
                eptr->getSS()<<str;\
                logger->debug(eptr) ;}
        
#define ZYX_LOG_DEBUG(logger,str) ZYX_LOG_LEVEL_DEBUG(logger, zyx::LogLevel::DEBUG,str)
```
