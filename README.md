# UltraLog
One of the best way to write log, can re-locate the output to different device such as screen/file/wechat/mail/sms

 All of the function was packaged to a DLL, you can use it very simple.
 
 // Get one of instance of UltraLog, used singlton design pattern.
 
  extern "C" LOGDLL_DECLARE ILog *  GetLogInstance();
  
  // add a new log output device to Ultralog, all of the deivce would be outputted to.
  
  extern "C" LOGDLL_DECLARE void AddNewLog(ILog * pNewLog);
  
  // remove the log instance, called at the exit of caller.
  
  extern "C" LOGDLL_DECLARE void RemoveLogInstance();
  
  
write a log to output device as the following
  
  usage:  
  
   ILog * theLog = GetLogInstance();
   
   theLog->AddRecord("Hello, world", ILog::LogInfo);
    
   virtual VOID AddRecord(LPCTSTR szMessage, LogType type= logInfo) = NULL;
   
   virtual VOID AddRecord(LogType type , LPCTSTR lpszFormat, ...) = NULL;
	
	
the log output level is the below.
 
 char *szAllLevel[] = {"debug", "info". "warning", "error", "fatal"};
 
 which is defined in config file.
 
 the sample of config file is the below , which is als can be merged to the other ini file of caller.
 
 
 
 
 [Log]
 
Output = File, Screen, Mail , SMS


[FileLog]

FileName = E:\Kiwa-tech\log\gate.log

MaxSize = 5120 K

LogLevel = Debug

Remark = 


[ScreenLog]

ListBoxName = LogListBox

MaxSize = 1024

LogLevel = Debug

Remark = 


[MailLog]

SMTPServer = 163.com

User = test

password = test

LogLevel = Debug

GroupSize = 128

Remark = 


[SMSLog]

MoblieNo = 15800888888

LogLevel = Fatal

GroupSize = 1

Remark = 


[WeChatLog]

Group = 

Account = 

LogLevel = Fatal


