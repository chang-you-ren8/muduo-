- 用户自定义outputFunc和flushFunc
- 用户（比如测试程序）调用setOutputFunc, setFlushFunc, setLoggerLevel 来设置如何将日志内容写到文件中,同时设置日志等级
- .然后调用LOG_INFO, LOG_DEBUG。。。。。。


**那么在哪里使用了异步日志呢？**
- 实际上在outputFunc中使用异步日志：创建一个全局的AsyncLogger对象，然后在outputFunc中调用AsyncLogger的append函数来将日志写到文件中
- 每当LOG_INFO, LOG_DEBUG结束后，都会在Logger对象的析构函数中调用全局的outputFunc。每个LOG宏都会创建一个匿名对象

