#include "TFile.h"
#include "TSocket.h"
#include "TMessage.h"
#include "TROOT.h"
#include "TTree.h"
#include <iostream>
#include "RootEventData/TBossFullEvent.h"

using namespace std;

void hclient(char *argv,char *argv1)
{

   TEvtHeader *event=new TEvtHeader();

   //gROOT->ProcessLine(".L libthis2.so");

   // Open connection to server
   TSocket *sock = new TSocket("localhost", 51400);

   //wait till we get the start message
   char str[32];
   sock->Recv(str,32);


   //server tells us who we are
   //int idx=!strcmp(str,"go 0") ? 0 : 1;
   int idx;
   if(strcmp(str,"go 0") == 0){
      idx = 0;
    }

   if (idx == 0){
   printf("this is the client:\n");
   
    
   //open the file and get the object
   if(strcmp(argv,"") != 0){
      //TFile f1("1.dst");
      //TObject* obj1=f1.Get("Event");
/*      TFile *f = new TFile(argv);
      f->Print();
      TTree *t = (TTree*)f->Get("Event");

      for ( int i=0;i<5;i++){
          TEvtHeader *event = new TEvtHeader();
          t->SetBranchAddress("TEvtHeader",&event);
          t->GetEntry(i);
          //cout<<event->getEventId()<<endl;
      
          //create TMessage to save the object
          TMessage mess(kMESS_OBJECT);
          mess.WriteObject(event);
          event->Print();

          // send message
          sock->Send(mess);
     }*/
	  TMessage *mess;
//TMessage *mess1;

	  printf("%s,%s \n",argv,argv1);
	  sock->Send(argv);
	  sock->Send(argv1);

          sock->Recv(mess);
//free(mess);
//sock->Recv(mess1);
          //TObject* m= mess->ReadObject(mess->GetClass());
          TTree* m= (TTree*)mess->ReadObject(mess->GetClass());
          m->Show(0);
//TObject* m1= mess1->ReadObject(mess1->GetClass());
          m->Print();
//m1->Print();
	  sock->Send("Finished");
   }

   //sock->Send("the first client:\n");

  
  }
    // Close the socket
    sock->Close();
  
}

int main(int argc, char **argv){
	hclient(argv[1],argv[2]);
        return 0;
}
