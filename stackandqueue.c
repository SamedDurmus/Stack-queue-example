/*
*************************************



    3 ayrı yaş grubu sırayla kuyruğa girdiği için ve en öndeki (sıraya giren ilk kişi) kişinin
ilk hizmet edilip kuyruktan ve sıradan çıkarılması gerektiği için Queue yapısı seçtim.

    struct yapısının içerisine linked list ile oluşturduğum Queue yapılarını kodladım.
Bunun sebebi 3 farklı kuyruk için 3 farklı Queue yapısı yazmadan nesne oluşturma mantığı ile
struct yapısından 3 farklı obje oluşturup 3 farklı queue yapısını elde ettim.

    Dondurmacımızda gün sonunda hizmet edilen bütün müşterileri sondan başa olacak şekilde kaydetmek
istediği için bu kısımda da Stack yapısını tercih ettim.Çünkü Stack yapısında Queue yapısından
farklı olarak ilk giren Stackin en sonuna , son giren ise stackin en başında kalıyor. Stack
içerisinde ki elemanları bastır dediğimde olduğu gibi ödevde istenilen tersen sıralamayı elde ettim.

    Son olarak inputtan komut okuma kısmında öncelikle her satırın birinci kelimesi olan newcustomer veya
servecustomer kelimelerini tespit etmeye çalıştım. İlk kontrolü tamamladıktan sonra iki farklı
fonksiyon için 2 farklı kontrol aşamaları yapıp satırın kalanında kalan bilgileri alıp programı
düzgün outputu verir hale getirdim.





*************************************
*/
#include <stdio.h>
#include <stdlib.h>
FILE *filepointer;

// içerisine isim ve yaşgrubu bilgisini alan bir struct oluşturdum
// nodeStack* next komutu ile bir sonraki node'a gidecek pointer ı belirledim.
struct nodeStack {
    char nameCustomer[10];
    char ageCustomer[1];
    struct nodeStack* next;
};

// nodeStack yapısının bilgilerini kullanarak yeni bir stack oluşturan yapıyı hazırladım.
struct nodeStack* newNodeStack(char *nameCustomer, char *ageCustomer){
    struct nodeStack* nodeS = malloc(sizeof(struct nodeStack));
    strcpy(nodeS->nameCustomer,nameCustomer);
    strcpy(nodeS->ageCustomer,ageCustomer);
    nodeS->next = NULL;
    return nodeS;
}

//  newNodeStack fonksiyonunu çağırarak parametre olarak aldığı name ve age
// bilgilerini stacke ekleme yaptırdım.
void push(struct nodeStack** root, char *nameCustomer,char *ageCustomer){
    struct nodeStack* nodeS = newNodeStack(nameCustomer,ageCustomer);
    nodeS->next = *root;
    *root = nodeS;

}
// pop fonksiyonunda stackin içinde pointerla gezerek en üstten başlayıp
// stack içinde ki elementleri stackten çıkartma işlemi yaptırdım
// en üsttekini stackten çıkarırken, bilgilerinede ulaşıp bilgilerini output
// dosyasına yazdırdım.
void pop(struct nodeStack** root){
    struct nodeStack* temp = *root;
    *root = (*root)->next;
    filepointer=fopen("output.txt","w");
    while(temp!=NULL){
      fprintf(filepointer,"%s\t%s\n",temp->nameCustomer, temp->ageCustomer);
      temp=temp->next;

    }

    free(temp);


}

// stack için ilk node' u burada globalde oluşturup queue yapısının içerisinde kullandım.
struct nodeStack* root = NULL;


// isim ve yaş bilgileriyle birlikte bir sonraki node'a gidecek pointerın olduğu struct yapısı
struct nodeQue {

   char nameCustomer[10];
   char ageCustomer[1];
    struct nodeQue* next;
};
typedef struct nodeQue* nodeQQ;

// sırada ön ve arkadaki kişinin bilgilerini gösteren front ve rear pointerları
struct Queue {
    struct nodeQue *front, *rear;
};
typedef struct Queue* nodeqq;

//  nodeQue yapısını ve bilgilerini kullanarak yeni bir link list oluşturup bilgileri
// işleyen fonksiyon. newNodQ.
// char * pointer kullanmından önce char [] kullanmayı denemedim fakat çalışmadı.
struct nodeQue* newNodeQ(char *nameCustomer, char *ageCustomer){
    struct nodeQue* temp = malloc(sizeof(struct nodeQue));

    strcpy(temp->nameCustomer,nameCustomer);
	strcpy(temp->ageCustomer,ageCustomer);
    temp->next = NULL;
    return temp;
}

// yeni bir queueu yapısı oluştuyor.
struct Queue* createQueue(){
    struct Queue* q = malloc(sizeof(struct Queue));
    q->front = q->rear = NULL;
    return q;
}

// Ödevde istendiği içine aldığı parametlere göre belirtilen kuyrua gidip,
// parametrede verilen ismi ve yaş grubunu kuyruğa ekliyor.
void NewCustomer(struct Queue* q, char *nameCustomer, char *ageCustomer){

    struct nodeQue* temp = newNodeQ(nameCustomer,ageCustomer);

    if (q->rear == NULL) {
        q->front = q->rear = temp;
        return;
    }
    q->rear->next = temp;
    q->rear = temp;
}


// Verilen yaş grubuna göre gerekli kuyruğa gidip
// bir kişiye hizmet verip o kişiyi kuyruktan çıkaran fonksiyon.
void ServeCustomers(struct Queue* q,char *ageCustomer){

    //eğer boşşsa istenildiği gibi ** bastıran kısım.
    if (q->front == NULL) {

   		push(&root,"*****",ageCustomer);
        return;
	}
    // en öndeki kişiyi geçici bir node'a alıyoruz.
	struct nodeQue* temp = q->front;
    push(&root,q->front->nameCustomer, q->front->ageCustomer);
    // en öndeki kişiyi stack'e yolladıktan sonra(satır 147)
    // artık bir sonraki müşteriyi en öne alıyorum.
	q->front = q->front->next;
	//eğer en önde kimse yoksa arkasında da kimse yoktur diyerek nodeları temizliyorum
    if (q->front == NULL){
    	q->rear = NULL;
	}

    free(temp);
}


int main(){

    struct Queue* e = createQueue(); //E elder    3 tane kuyruk yapısını burada oluşturuyorum
    struct Queue* a = createQueue();//A adults
    struct Queue* c = createQueue();//C children

    char fonk[100];// newcustomer mı servecustomers mı satırın ilk kelimesini atadığım fonksiyon
    char agegrp[100]; // yaş grubu(E,A,C)
    char nameC[100];  //
    int serveAmount; // ServeCustomers'ın kaç kere çalışacağını belirleyen değişken


    filepointer = fopen("input.txt","r" );

    //ilk satırı okuyup hangi fonskiyona gideceğini karar veren kısım.
    while (fscanf(filepointer,"%s \n", fonk ) !=EOF ){

      if(strcmp(fonk, "ServeCustomers")==0){
            fscanf(filepointer,"%s %d \n", agegrp, &serveAmount ) ;
        // fonksiyona karar verdikten sonra hangi yaş grubuna ait queue ya gideceğini
        // kontrol eden kısım.
         if(strcmp(agegrp,"E")==0){
                for(int i=0;i<serveAmount;i++){
                    ServeCustomers(e,"E");
                }

         }
         else if(strcmp(agegrp,"A")==0){
                for(int i=0;i<serveAmount;i++){
                    ServeCustomers(a,"A");
                }

         }
         else if(strcmp(agegrp,"C")==0){
                for(int i=0;i<serveAmount;i++){
                    ServeCustomers(c,"C");
                }

         }

        }
       else if (strcmp(fonk, "NewCustomer")==0){
           fscanf(filepointer,"%s %s \n", agegrp, nameC ) ;
        // fonksiyona karar verdikten sonra hangi yaş grubuna ait queue ya gideceğini
        // kontrol eden kısım.
           if(strcmp(agegrp,"E")==0){
                NewCustomer(e,nameC,"E");

           }


           else if(strcmp(agegrp,"A")==0){
                NewCustomer(a,nameC,"A");

            }
           else if(strcmp(agegrp,"C")==0){
                NewCustomer(c,nameC,"C");

            }

        }


    }

    //stackte ki bilgileri bastıran satır.
    pop(&root);


    fclose(filepointer);


    return 0;
}

