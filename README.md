# UltraLog
One of the best way to write log, can re-locate the output to different device such as screen/file/wechat/mail/sms

 All of the function was packaged to a DLL, you can use it very easy.
 
 // Get one of instance of UltraLog, used singlton design pattern.
 
  extern "C" LOGDLL_DECLARE ILog *  GetLogInstance();
  
  // add a new log output device to Ultralog, all of the deivce which is defined in IniFile would be outputted to.
  
  extern "C" LOGDLL_DECLARE void AddNewLog(ILog * pNewLog);
  
  // remove the log instance, called at the exit of caller.
  
  extern "C" LOGDLL_DECLARE void RemoveLogInstance();
  
  
write a log to output device as the following
  
  usage:  
  
   ILog * theLog = GetLogInstance();
   
   theLog->AddRecord("Hello, world", ILog::LogInfo);
    
   or 
   LPSTR szMsg = "Hello, world";
   theLog->AddRecord(ILog:LogInfo, "the message is %s", szMsg);
   
   
   virtual VOID AddRecord(LPCTSTR szMessage, LogType type= logInfo) = NULL;
   
   virtual VOID AddRecord(LogType type , LPCTSTR lpszFormat, ...) = NULL;
	
	
the log output level is the below.
 
 char *szAllLevel[] = {"debug", "info". "warning", "error", "fatal"};
 
 which is defined in config file.
 
 the sample of config file is the below , which is als can be merged to the other ini file of caller.
 
 
 
 
 [Log]
 
Output = File, Screen, Mail , SMS	#print the log to File, Screen, mail and SMS, can be one of these



[FileLog]

FileName = E:\Kiwa-tech\log\gate.log	# the filename of the Log, can used absolute diectory or relative dirctory.

MaxSize = 5120 K	# the maximue size of the log file ,when over the maxsize, the log file will be backuped.

LogLevel = Debug

Remark =     #anythings you want to input.


[ScreenLog]

ListBoxName = LogListBox	#the controller which shown the log message.

MaxSize = 1024			# thea maximue lines of the listbox controller.

LogLevel = Debug

Remark = 


[MailLog]

SMTPServer = 163.com	 	# the smtp server of mail

User = test

password = test

LogLevel = Debug

GroupSize = 128

Remark = 


[SMSLog]

MoblieNo = 15800888888		#the moblile you used to send the log.

LogLevel = Fatal

GroupSize = 1

Remark = 


[WeChatLog]

Group = 			#which group you want to send log, can be the wechat account and wechat group

Account = 			#your account to used send the log

LogLevel = Fatal


