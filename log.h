#ifndef LOG_H
#define LOG_H
#include <string>
#include <stdint.h>
#include <memory>
#include <list>
#include <sstream>
#include <fstream>
#include <vector>
#include <stdarg.h>
#include <map>
#include<time.h>


/**
 * @brief ʹ����ʽ��ʽ����־����debug����־д�뵽logger
 */
#define ZYX_LOG_LEVEL_DEBUG(logger, level,str) \
        {\
        zyx::LogEvent::ptr eptr(new zyx::LogEvent(logger, level, \
                        __FILE__, __LINE__, 0, 0,\
                0, time(0), "root"));\
                eptr->getSS()<<str;\
                logger->debug(eptr) ;}
        

#define ZYX_LOG_DEBUG(logger,str) ZYX_LOG_LEVEL_DEBUG(logger, zyx::LogLevel::DEBUG,str)
  
 /**
  * @brief ʹ����ʽ��ʽ����־����info����־д�뵽logger
  */
#define ZYX_LOG_LEVEL_INFO(logger, level,str) \
        {\
        zyx::LogEvent::ptr eptr(new zyx::LogEvent(logger, level, \
                        __FILE__, __LINE__, 0, 0,\
                0, time(0), "root"));\
                eptr->getSS()<<str;\
                logger->info(eptr);}
        

#define ZYX_LOG_INFO(logger,str) ZYX_LOG_LEVEL_INFO(logger, zyx::LogLevel::INFO,str)

  /**
   * @brief ʹ����ʽ��ʽ����־����warn����־д�뵽logger
   */
#define ZYX_LOG_LEVEL_WARN(logger, level,str) \
        {\
        zyx::LogEvent::ptr eptr(new zyx::LogEvent(logger, level, \
                        __FILE__, __LINE__, 0, 0,\
                0, time(0), "root"));\
                eptr->getSS()<<str;\
                logger->warn(eptr);}  

#define ZYX_LOG_WARN(logger,str) SYLAR_LOG_LEVEL(logger, zyx::LogLevel::WARN,str)

   /**
    * @brief ʹ����ʽ��ʽ����־����error����־д�뵽logger
    */
#define ZYX_LOG_LEVEL_ERROR(logger, level,str) \
        {\
        zyx::LogEvent::ptr eptr(new zyx::LogEvent(logger, level, \
                        __FILE__, __LINE__, 0, 0,\
                0, time(0), "root"));\
                eptr->getSS()<<str;\
                logger->error(eptr);}  
#define ZYX_LOG_ERROR(logger,str) ZYX_LOG_LEVEL_ERROR(logger, zyx::LogLevel::ERROR,str)

    /**
     * @brief ʹ����ʽ��ʽ����־����fatal����־д�뵽logger
     */
#define ZYX_LOG_LEVEL_FATAL(logger, level,str) \
        {\
        zyx::LogEvent::ptr eptr(new zyx::LogEvent(logger, level, \
                        __FILE__, __LINE__, 0, 0,\
                0, time(0), "root"));\
                eptr->getSS()<<str;\
                logger->error(eptr);}  
#define ZYX_LOG_FATAL(logger,str) ZYX_LOG_LEVEL_FATAL(logger, zyx::LogLevel::FATAL,str)

namespace zyx
{
class LogFormatter;
class Logger;

class LogLevel 
{
public:
    /**
        * @brief ��־����ö��
        */
    enum Level {
        /// δ֪����
        UNKNOW = 0,
        /// DEBUG ����
        DEBUG = 1,
        /// INFO ����
        INFO = 2,
        /// WARN ����
        WARN = 3,
        /// ERROR ����
        ERROR = 4,
        /// FATAL ����
        FATAL = 5
    };

    /**
        * @brief ����־����ת���ı����
        * @param[in] level ��־����
        */
        static const char* ToString(LogLevel::Level level);

        /**
            * @brief ���ı�ת������־����
            * @param[in] str ��־�����ı�
            */
         static LogLevel::Level FromString(const std::string& str);
};

class LogEvent
{
public:
    typedef std::shared_ptr<LogEvent> ptr;
    LogEvent(std::shared_ptr<Logger> logger, LogLevel::Level level
        , const char* m_file, int32_t m_line, uint32_t m_elapse, uint32_t m_threadId,
        uint32_t m_fiberId, uint64_t m_time, std::string m_threadName);
    const char* getFile() const { return m_file; }
    int32_t getLine() const { return m_line; }
    uint32_t getElapse() const { return m_elapse; }
    uint32_t getThreadId() const { return m_threadId; }
    uint32_t getFiberId() const { return m_fiberId; }
    uint64_t getTime() const { return m_time; }
    const std::string& getThreadName() const { return m_threadName; }
    std::string getContent() const { return m_ss.str(); }
    std::stringstream& getSS() { return m_ss; }

private:
    /// �ļ���
    const char* m_file = nullptr;
    /// �к�
    int32_t m_line = 0;
    /// ����������ʼ�����ڵĺ�����
    uint32_t m_elapse = 0;
    /// �߳�ID
    uint32_t m_threadId = 0;
    /// Э��ID
    uint32_t m_fiberId = 0;
    /// ʱ���
    uint64_t m_time = 0;
    /// �߳�����
    std::string m_threadName;
    /// ��־������
    std::stringstream m_ss;
    /// ��־��
    std::shared_ptr<Logger> m_logger;
    /// ��־�ȼ�
    LogLevel::Level m_level;
};

class LogFormatter
{
public:
    typedef std::shared_ptr<LogFormatter> ptr;
    LogFormatter(std::string formate="%D %r-%p-%t %m%n");
    std::ostream& format(std::ostream& ofs, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event);
    void init(); //��ģ���ʽ���н���
    class FormatItem {
    public:
        typedef std::shared_ptr<FormatItem> ptr;
        /**
         * @brief ��������
         */
        virtual ~FormatItem() {}
        /**
         * @brief ��ʽ����־����
         * @param[in, out] os ��־�����
         * @param[in] logger ��־��
         * @param[in] level ��־�ȼ�
         * @param[in] event ��־�¼�
         */
        virtual void format(std::ostream& os, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) = 0;
    };
private:
    
    /// ��־��ʽģ��
    std::string m_pattern;
    /// ��־��ʽ�������ʽ
    std::vector<FormatItem::ptr> m_items;
};

class LogAppender
{
public:
    typedef std::shared_ptr<LogAppender> ptr;
    virtual void log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) = 0;
protected:
    LogFormatter::ptr m_formatter;

};

class Logger : public std::enable_shared_from_this<Logger>
{
public:
    typedef std::shared_ptr<Logger> ptr;
    Logger(LogLevel::Level my_level, LogFormatter::ptr m_formatter);
    void log(LogLevel::Level level, LogEvent::ptr event);
    void debug(LogEvent::ptr event);
    void info(LogEvent::ptr event);
    void warn(LogEvent::ptr event);
    void error(LogEvent::ptr event);
    void fatal(LogEvent::ptr event);
    void addAppender(LogAppender::ptr appender);
    void delAppender(LogAppender::ptr appender);
    LogLevel::Level getLevel() { return my_level; }
private:
    LogLevel::Level my_level = zyx::LogLevel::DEBUG;
    LogEvent::ptr event;
    LogFormatter::ptr m_formatter;
    std::vector<LogAppender::ptr> m_appenders;
};

class StdoutLogAppender : public LogAppender
{
public:
    typedef std::shared_ptr<StdoutLogAppender> ptr;
    StdoutLogAppender(LogFormatter::ptr p) { m_formatter = p; }
    void log(Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override;
};

class FileLogAppender : public LogAppender
{
public:
    typedef std::shared_ptr<FileLogAppender> ptr;
    FileLogAppender(const std::string filename, LogFormatter::ptr p);
    void log(Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override;
private:
    // �ļ�·��
    std::string m_filename;
    // �ļ���
    std::ofstream m_filestream;
    // �ϴ����´�ʱ��
    uint64_t m_lastTime = 0;
};

class LoggerManager
{
public:
    typedef std::shared_ptr<LoggerManager> ptr;
    LoggerManager(LogLevel::Level level= LogLevel::Level::DEBUG,bool write_std=true, bool write_file=true);
    LoggerManager(LogLevel::Level level, bool write_std, std::vector<std::string>write_file, std::string formate);
    void add_file(std::string file_name);
    void clear_file();
    Logger::ptr Getlogger() { return logger_m; }

private:
    LogLevel::Level level = LogLevel::Level::DEBUG;
    LogEvent::ptr event;
    LogFormatter::ptr logformate;
    Logger::ptr logger_m;
    std::vector<std::string> file = {"test.txt"};
};

}

#endif




