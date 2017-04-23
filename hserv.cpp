#include "TFile.h"
#include "TServerSocket.h"
#include "TSocket.h"
#include "TMessage.h"
#include "TMonitor.h"
#include "TROOT.h"
#include "TTree.h"
#include "TChain.h"
#include "RootEventData/TEvtHeader.h"
#include "RootEventData/TMcEvent.h"
#include "RootEventData/TEvtRecObject.h"
#include "RootEventData/TDstEvent.h"
#include "RootEventData/TEvtNavigator.h"
#include "RootEventData/TTrigEvent.h"

void hserv() 
{
   TEvtHeader *event=new TEvtHeader();
   TMcEvent *event1=new TMcEvent();

   time_t nowTime1;
   time(&nowTime1);
   printf("At now time is: %s\n",ctime(&nowTime1));

   //gROOT->ProcessLine(".L libthis2.so");
   TServerSocket *ss = new TServerSocket(51400, kTRUE);

   // Accept a connection and return a full-duplex communication socket.
   TSocket *s0 = ss->Accept();

   //tell the clients to start
   s0->Send("go 0");

   //Close the server socket
   //ss->Close();

   TMonitor *mon=new TMonitor;
   
   mon->Add(s0);
  
   TSocket *s;
   s = mon->Select();
   char name[64];

   while (1) {
       TMessage *mess;
//TMessage *mess1;
     
       s->Recv(mess);
//s->Recv(mess1);
       if(mess->What() == kMESS_STRING ){
          	char str[64];
//char str1[64];
          	mess->ReadString(str,64);
//mess1->ReadString(str1,64);
//mess1->ReadString(str,64);
	  if(strcmp(str,"Finished")==0){
          	mon->Remove(s);
          	if (mon->GetActive() == 0) {
            		printf("No more active clients... waitting\n");

   			// Accept a connection and return a full-duplex communication socket.
   			TSocket *s1 = ss->Accept();

   			//tell the clients to start
   			s1->Send("go 0");
   			
			mon->Add(s1);
			s = mon->Select();
            		//break;
          		}
	  }else{
printf("%s","test......\n");
          	//printf("%s   \n", str);
//printf("%s   \n", str1);
		int tag=0;
		for(int i=0;i<strlen(str);i++)
			if(str[i]<='0'||str[i]>='9')
				{tag=1;break;}
		if(tag==1)
			strcpy(name,str);
		else{
		//printf("%s   \n",name);
		TFile *f = new TFile(name);
		//f->Print();
      		//TTree *t = (TTree*)f->Get("Event");
		int temp=atoi(str);
                TChain old_chain("Event");
		old_chain.Add(name);
		//
                TTree* new_tree = old_chain.CloneTree(0);
		old_chain.GetEntry(temp);


		
		//printf("%d   \n",temp);

          		//TEvtHeader *event = new TEvtHeader();
          		//t->SetBranchAddress("TEvtHeader",&event);
			
                        //TEvtNavigator *event1=new TEvtNavigator();
                        //t->SetBranchAddress("EventNavigator",&event1);

                        //TMcEvent *event2=new TMcEvent();
                        //t->SetBranchAddress("TMcEvent",&event2);

                        //TDstEvent *event3 = new TDstEvent();
          		//t->SetBranchAddress("TDstEvent",&event3);

                        //TEvtRecObject *event4 = new TEvtRecObject();
          		//t->SetBranchAddress("TEvtRecObject",&event4);

          		
                        new_tree->Fill();

          		//cout<<event->getEventId()<<endl;
      			TMessage mess(kMESS_OBJECT);
//TMessage mess1(kMESS_OBJECT);
			mess.WriteObject(new_tree);
//mess.WriteObject(event);
          		//create TMessage to save the object
			
                        new_tree->Print();
                        new_tree->Show(0);
                        //event->Print();
			s->Send(mess);
//s->Send(mess1);
          // send message
     			
	    }
	  }
       }

       delete mess;
//delete mess1;


       if(s == s0){
          printf("Client 1: bytes recv = %d, bytes sent = %d\n", s0->GetBytesRecv(),s0->GetBytesSent());
       }


       // Close the socket.
       

       time_t nowTime2;
       time(&nowTime2);
       printf("At now time is: %s\n",ctime(&nowTime2));
       }
     s->Close();
ss->Close();

  }

int main(int argc, char **argv){
	hserv();
}
