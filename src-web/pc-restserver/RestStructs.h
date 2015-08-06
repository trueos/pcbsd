// ===============================
//  PC-BSD REST API Server
// Available under the 3-clause BSD License
// Written by: Ken Moore <ken@pcbsd.org> July 2015
// =================================
#ifndef _PCBSD_REST_SERVER_REST_STRUCTS_H
#define _PCBSD_REST_SERVER_REST_STRUCTS_H
#include <QString>
#include <QStringList>
#include <QDateTime>


//NOTE: The input structure parsing assumes a JSON input body
class RestInputStruct{
public:
	QString VERB, URI, HTTPVERSION;
	QStringList Header;
	QString Body;

	RestInputStruct(QString message){
	  Header = message.section("\n{",0,0).split("\n");
	  Body = message.remove(Header.join("\n")+"\n"); //chop the headers off the front
	  if(!Header.isEmpty()){
	    QString line = Header.takeFirst(); //The first line is special (not a generic header)
	    VERB = line.section(" ",0,0);
	    URI = line.section(" ",1,1);
	    HTTPVERSION = line.section(" ",2,2);
	  }
	}
	~RestInputStruct(){}
		
};

class RestOutputStruct{
public:
	enum ExitCode{OK, CREATED, ACCEPTED, NOCONTENT, RESETCONTENT, PARTIALCONTENT, PROCESSING, BADREQUEST, UNAUTHORIZED, FORBIDDEN, NOTFOUND };
	QString HTTPVERSION;
	QStringList Header;
	ExitCode CODE;
	QString Body;
	
	RestOutputStruct(){
	  CODE = BADREQUEST; //default exit code
	}
	~RestOutputStruct(){}
		
	QString assembleMessage(){
	  QStringList headers;
	  QString firstline = HTTPVERSION;
	  switch(CODE){
	    case PROCESSING:
	      firstline.append(" 102 Processing"); break;
	    case OK:
	      firstline.append(" 200 OK"); break;
	    case CREATED:
	      firstline.append(" 201 Created"); break;
	    case ACCEPTED:
	      firstline.append(" 202 Accepted"); break;
	    case NOCONTENT:
	      firstline.append(" 204 No Content"); break;
	    case RESETCONTENT:
	      firstline.append(" 205 Reset Content"); break;
	    case PARTIALCONTENT:
	      firstline.append(" 206 Partial Content"); break;
	    case BADREQUEST:
	      firstline.append(" 400 Bad Request"); break;
	    case UNAUTHORIZED:
	      firstline.append(" 401 Unauthorized"); break;
	    case FORBIDDEN:
	      firstline.append(" 403 Forbidden"); break;
	    case NOTFOUND:
	      firstline.append(" 404 Not Found"); break;
	  }
	  headers << firstline;
	  headers << "Date: "+QDateTime::currentDateTime().toString(Qt::ISODate);
	  //Add other headers here as necessary
	  if(!Header.isEmpty()){ headers << Header; }
	  //Now add the body of the return
	  headers << Body;
	  return headers.join("\n");
	}
};

#endif
