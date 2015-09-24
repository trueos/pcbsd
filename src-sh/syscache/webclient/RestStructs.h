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

#define CurHttpVersion QString("HTTP/1.1")

//NOTE: The input structure parsing assumes a JSON input body
//NOTE: Common VERB's are:
/*	GET - Read a resource (no changes made)
	PUT - Insert/update a resource (makes changes - nothing automatically assigned for a new resource)
	POST - Insert/update a resource (makes changes - automatically assigns data for new resource as necessary)
	DELETE - Remove a resource (makes changes)
	OPTIONS - List the allowed options on a resource (no changes made)
	HEAD - List the response headers only (no changes made)
*/
class RestInputStruct{
public:
	QString VERB, URI, HTTPVERSION;
	QStringList Header;
	QString Body;

	RestInputStruct(QString message){
	  HTTPVERSION = CurHttpVersion;
	  if(!message.startsWith("{")){ 
	    Header = message.section("\n{",0,0).split("\n");
	  }
	  if(!Header.isEmpty()){
	    QString line = Header.takeFirst(); //The first line is special (not a generic header)
	    VERB = line.section(" ",0,0);
	    URI = line.section(" ",1,1);
	    HTTPVERSION = line.section(" ",2,2);
	    Body = message.remove(Header.join("\n")+"\n"); //chop the headers off the front
	  }else{
	    //simple bypass for any non-REST inputs - just have it go straight to JSON parsing
	    VERB = "GET"; 
	    URI = "/syscache";
	    Body = message;
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
	  HTTPVERSION = CurHttpVersion;
	  CODE = BADREQUEST; //default exit code
	}
	~RestOutputStruct(){}
		
	QString assembleMessage(){
	  /* JUST OUTPUT RAW JSON - DISABLE REST FOR THE MOMENT
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
	  if(!Body.isEmpty()){ headers << "Content-Length: "+QString::number(Body.length()); }
	  headers << Body;
	  return headers.join("\n");*/
	  return Body;
	}
};

#endif
