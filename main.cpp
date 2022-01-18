#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

struct  Dia_hora{
    int dia;
    int mes;
    int any;
    int h;
    int m;
};
const int EOS=0;
struct Concert{
    Dia_hora inici;
    char entrada;
    string lloc;
    string esdev;
    string grup;
    bool interes;
};

struct Grup{
    string nom;
    bool actiu;
    int n_fets;
    int n_anat;
};

const int MAX_C=1000;
const int MAX_G=60;
typedef Concert Vector_concerts[MAX_C];
struct V_concerts{
    Vector_concerts t;
    int n;
};

typedef Grup Vector_grups[MAX_G];
struct V_grups{
    Vector_grups t;
    int n;
};

void obrir_fitxer(ifstream &f_in){
//Pre: cert
//Post: retorna una variable ifstream associada al primer fitxer existent de nom llegit de teclat
    string nom;
    cin>>nom;
    f_in.open(nom.c_str());
    while(not f_in.is_open()){
        f_in.clear();
        cout<<"No s'ha pogut obrir. Entra un altre nom: ";
        cin>>nom;
        f_in.open(nom.c_str());
    }
}

void inserir_grup(V_grups& grups, Grup nou){
//Pre: grups.n=N, 0<=N<MAX_G i grups.t[0..N-1] ordenat alfabèticament per grup
//Post: s'ha inserit el grup nou a grups respectant l'ordre alfabètic i grups.n=N+1
    int i=grups.n-1;
    while(i>=0 and nou.nom < grups.t[i].nom){
        grups.t[i+1]=grups.t[i];
        i--;
    }
    grups.t[i+1]=nou;
    grups.n++;
}

Grup inicialitza(){
//Pre: cert
//Post: retorna un grup amb tots els camps enters inicialitzats a 0 i actiu a true
    Grup g;
    g.n_anat=0;
    g.n_fets=0;
    g.actiu=true;
    return g;
}


void mostrar_grup(Grup g){
//Pre: --
//Post: mostra la informació de g
    if(g.actiu) cout<<" + ";
    else cout<<" - ";
    cout<<left<<setw(25)<<setfill(' ')<<g.nom<<" "<<setw(3)<<setfill(' ')<<right<<g.n_anat<<"/"<<g.n_fets<<endl;//<<": "<<g.nom<<endl;

}

void mostrar_estadistiques_grups(const V_grups &grups){
//Pre: 0<=grups.n<=MAX_G
//Post: mostra grups.t[0..grups.n-1]
    cout<<"----- GRUPS D'INTERES -----"<<endl;
    for(int i=0;i<grups.n;i++)
        mostrar_grup(grups.t[i]);
}

void omplir_vector_grups( V_grups &grups){
//Pre: cert
//Post: 0<=grups.n<=MAX_G i grups.t[0..grups.n-1] conté els grups del fitxer introduït per teclat ordenats alfabèticament
    cout<<"Nom del fitxer dels grups: ";
    ifstream f_in;
    obrir_fitxer(f_in);//,"grups.txt");
    grups.n=0;
    Grup grup_nou=inicialitza();
    f_in>>grup_nou.nom;
    while(not f_in.eof() and grups.n<MAX_G){
        inserir_grup(grups,grup_nou);
        f_in>>grup_nou.nom;
    }
}


Concert llegir_concert(ifstream& f_in){
//Pre: f_in obert i no acabat
//Post: es retorna el concert llegit de f_in, pot ser buit si després de la 1a lectura es detecta el final d'f_in
    Concert c;
    f_in>>c.inici.dia;
    if(not f_in.eof())
        f_in>>c.inici.mes>>c.inici.any>>c.inici.h>>c.inici.m>>c.entrada>>c.esdev>>c.lloc>>c.grup;
    return c;
}

Concert llegir_concert(){
//Pre: cert
//Post: es retorna el concert llegit de teclat, si al llegir el dia es detecta l'EOS, el dia és EOS i els altres camps tenen valors indeterminats
    Concert c;
    cin>>c.inici.dia;
    if(c.inici.dia!=EOS)
        cin>>c.inici.mes>>c.inici.any>>c.inici.h>>c.inici.m>>c.entrada>>c.esdev>>c.lloc>>c.grup;
    return c;
}

Dia_hora llegir_dia(){
//Pre: cert
//Post: retorna el dia llegit de teclat
    Dia_hora d;
    cin>>d.dia>>d.mes>>d.any;
    d.h=0;d.m=0;
    return d;
}

bool passa_abans(Dia_hora a, Dia_hora b){
//Pre: cert
//Post: retorna cert si el dia i hora d'a és anterior que el de b (passa primer a que b)i fals altrament
    bool va_primer=a.any<b.any;
    if(a.any==b.any){
        va_primer=a.mes<b.mes;
        if(a.mes==b.mes){
            va_primer= a.dia<b.dia;
            if(a.dia==b.dia){
                va_primer=a.h<b.h;
                if(a.h==b.h) va_primer=a.m<b.m;
            }
        }
    }
    return va_primer;
}

bool mateix_dia(Dia_hora d1, Dia_hora d2){
//Pre: cert
//Post: retorna true si el dia de d1 és el mateix que el de d2 i fals altrament
    return d1.any==d2.any and d1.mes==d2.mes and d1.dia==d2.dia;
}

bool mateix_inici(Dia_hora d1, Dia_hora d2){
//Pre: cert
//Post: retorna true si el dia i hora d'inci de d1 coincideix amb el de d2 i fals altrament
    return mateix_dia(d1,d2) and d1.h==d2.h and d1.m==d2.m;
}

bool mateix_concert(Concert a, Concert b){
//Pre: cert
//Post: retorna true si a i b es fan al mateix dia i hora d'inici i pel mateix grup i fals altrament
    return a.grup==b.grup and mateix_inici(a.inici,b.inici) ;
}


bool es_menor(Concert c1, Concert c2, char criteri){
//Pre: criteri és 'G','L', 'D' o 'E'
//Post: retorna true si c1 és menro que c2 tenint en compte el grup si criteri és 'G', el lloc si és 'L', la data d'inici si 'D' o l'esdeveniment si 'E'
    bool menor = c1.grup<c2.grup;
    if(criteri=='G'){
        if(c1.grup==c2.grup){
            menor=passa_abans(c1.inici,c2.inici);
        }
    }
    else if(criteri=='L'){
        if(c1.lloc==c2.lloc){
            if(c1.grup==c2.grup) menor=passa_abans(c1.inici,c2.inici);
        }
        else menor=c1.lloc<c2.lloc;
    }
    else if(criteri=='D'){
        if(not mateix_inici(c1.inici,c2.inici)) menor=passa_abans(c1.inici,c2.inici);
    }
    else if(criteri=='E'){
        if(c1.esdev==c2.esdev){
            if(c1.grup==c2.grup) menor=passa_abans(c1.inici,c2.inici);
        }
        else menor=c1.esdev<c2.esdev;
    }
    return menor;
}

void inserir(Concert nou, V_concerts& programats, char ordre){
//Pre: concers.n=N, 0<=N<MAX_C i programats.t[0..programats.n-1] ordenat segons ordre
//Post: s'ha afegit nou a programats ordenadament segons el criteri ordre
    int i=programats.n-1;
    while(i>=0 and es_menor(nou,programats.t[i], ordre)){
        programats.t[i+1]=programats.t[i];
        i--;
    }
    programats.t[i+1]=nou;
    programats.n++;
}

void apareix_i_pos(string grup, const V_grups& grups, bool& trobat, int& pos){
//Pre: 0<=grups.n<=MAX_P i grups.t[0..grups.n-1] ordenat alfabèticament per grup
//Post: trobat és true si per algun 0<=pos<N grups.t[pos].grup==grup i fals altrament, si trobat és fals pos és la posició on hauria d'aparèixer el grup si apareixés a grups ordenadament.
    int esq=0, dret=grups.n-1;
    trobat=false;
    while(esq<=dret and not trobat){
        int mig=(esq+dret)/2;
        if(grups.t[mig].nom==grup){trobat=true; pos=mig;}
        else if(grups.t[mig].nom<grup) esq=mig+1;
        else dret=mig-1;
    }
    if(not trobat) pos=esq;
}


void afegir(Concert nou,V_concerts& programats,V_grups &grups){
//Pre: NC=programats.n i NG=grups.n i 0<=NC<=MAX_C i 0<=NG<=MAX_G i programats.t[0..NC-1] ordenat per grup i data i grups.t[0..NG-1] ordenat per grup alfabèticament
//Post: s'ha afegit el concert nou a programats i s'ha comptat a les estadístiques de grups si hi cap, si nou.grup no aparaixia a grups.t[0...NG-1] i NG<MAX_G, s'ha inserit el nou grup i grups.n=NG+1
    bool trobat;
    int pos;
    apareix_i_pos(nou.grup,grups,trobat,pos);
    nou.interes=false;
    if(trobat) nou.interes=grups.t[pos].actiu;
    if(trobat and grups.t[pos].actiu) grups.t[pos].n_fets++;
    if(programats.n<MAX_C) inserir(nou, programats,'G');
}

void omplir_vector_concerts(V_grups& grups, V_concerts &programats){
//Pre: cert
//Post: programats conté els programats que hi havia guardats al fitxer indicat per teclat i les estadístiques guardadaes a grups.t[0..grup.n-1] s'han actualitzat
    cout<<"Nom del fitxer dels concerts: ";
    ifstream f_in;
    obrir_fitxer(f_in);//,"concerts.txt");
    programats.n=0;
    Concert nou = llegir_concert(f_in);
    while(not f_in.eof()){
        afegir(nou,programats,grups);
        nou=llegir_concert(f_in);
    }
}

void mostrar_concert(Concert c){
//Pre: --
//Post: mostra c
    if(c.interes) cout<<" + ";
    else cout<<" - ";
    cout<<setw(25)<<setfill(' ')<<left<<c.grup<<" "<<setw(2)<<right<<setfill('0')<<c.inici.dia<<"-"<<setw(2)<<c.inici.mes<<"-"<<c.inici.any<<" "<<setw(2)<<c.inici.h<<":"<<setw(2)<<c.inici.m;
    if(c.entrada=='S') cout<<" P ";
    else cout<<" G ";
    cout<<c.esdev<<" "<<c.lloc<<endl;
}

void mostrar_concerts(const V_concerts& concerts, char op){
//Pre: 0<= concerts.n<=MAX_C
//Post: mostra concerts.t[0..concerts.n-1]
    if(op=='P') cout<<"----- CONCERTS PROGRAMATS -----"<<endl;
    else if(op=='C') cout<<"----- CONCERTS CONFIRMATS -----"<<endl;
    for(int i=0; i<concerts.n;i++){
        mostrar_concert(concerts.t[i]);
    }
}

void afegir_noves_programacions(V_concerts& programats, V_grups& grups){
//Pre: 0<=programats.n<=MAX_C  i 0<=grups.n<=MAX_G
//Post: s'han afegit noves programacions a grups i programats mantenit els ordres respectius
    cout<<"Introdueix els concerts nous ("<<EOS<<" per acabar):"<<endl;
    cout<<"data (d m a) hora d'inici (h m) entrada (S/N) lloc esdeveniment grup"<<endl;
    Concert nou = llegir_concert();
    while(nou.inici.dia!=EOS){
        afegir(nou,programats,grups);
        nou=llegir_concert();
    }
}

Concert llegir_grup_i_inici(){
//Pre: cert
//Post: es retorna el Concert llegit de teclat.
    Concert nou;
    cout<<"Dades del concert (grup, data, i hora d'inici): ";
    cin>>nou.grup>>nou.inici.dia>>nou.inici.mes>>nou.inici.any>>nou.inici.h>>nou.inici.m;
    return nou;
}

void apareix_i_pos(const V_concerts &concerts, Concert c,char ordre, bool& trobat, int &pos){
//Pre: 0<=concerts.n<MAX_C concerts.t[0..concerts.n-1] ordenat per ordre
//Post: trobat és cert si existeis un 0<=pos<concers.n pel qual concerts.t[pos] sigui c, altrament trobat és fals i pos és la posició on hauria d'estar c si estigués a concerts.t respectant l'ordre indicat a ordre.
    int esq=0, dret=concerts.n-1;
    trobat=false;
    while(esq<=dret and not trobat){
        int mig=(esq+dret)/2;
        if(mateix_concert(c,concerts.t[mig])){trobat=true; pos=mig;}
        else if(es_menor(c,concerts.t[mig],ordre)) dret=mig-1;
        else esq=mig+1;
    }
    if(not trobat) pos=esq;
}

void actualitzar_interes_concerts(V_concerts& programats, V_concerts& confirmats, Grup& g){
//Pre: 0=programats.n<=MAX_C i 0<=confirmats.n<=MAX_C i programats.t[0..programats.n-1] ordenat per grup i data i hora del concert i confirmats.t[0..confirmats.n-1] per data
//Post: s'ha actualitat l'interès de tots els concerts de programats.t[0..programats.n-1] i confirmats.t[0..confirmats.n-1] del grup g d'acord amb l'interès de g, les estadístiques de g s'han actualizat d'acord amb programats i confirmats
    Concert concert_g;
    concert_g.inici.any=0;
    concert_g.grup=g.nom;
    //s'actualitzen els concerts programats comptant els que canvien
    bool trobat;
    int pos;
    apareix_i_pos(programats, concert_g, 'G', trobat,pos);
    int n_prog=0;
    while(pos<programats.n and programats.t[pos].grup==g.nom){
        programats.t[pos].interes=g.actiu;
        n_prog++; pos++;
    }
    //s'actulitzen els concerts confirmats comptant els que canvien
    int n_conf=0;
    for(int i=0; i<confirmats.n;i++){
        if(confirmats.t[i].grup==g.nom){
            confirmats.t[i].interes=g.actiu;
            n_conf++;
        }
    }
    n_prog+=n_conf;
    //s'actualitzen les estadístiques del grup
    if(g.actiu){
        g.n_anat+=n_conf;
        g.n_fets+=n_prog;
    }
    else{
        g.n_anat-=n_conf;
        g.n_fets-=n_prog;
    }
}


void actualitzar_grups(string grup, V_grups& grups,V_concerts& programats, V_concerts& confirmats){
//Pre: 0<=programats.n<=MAX_C i 0<=grups.n<=MAX_G i grups.t[0..grups.n-1] ordenat alfabèticament per grup
//Post: s'ha actualitzat grups insritn grup si no hi era o canviant l'interès de grup si hi era, s'ha actualtizat programats, confirmats i les estadístiques de grup en conseqüència
    Grup g=inicialitza();
    g.nom=grup;
    bool apareix;
    int pos;
    apareix_i_pos(g.nom,grups,apareix,pos);
    if(apareix) grups.t[pos].actiu=not grups.t[pos].actiu;
    else if(grups.n<MAX_G) inserir_grup(grups,g);
    actualitzar_interes_concerts(programats,confirmats,grups.t[pos]);
}

void elimina_concert(V_concerts &vec, int pos){
//Pre: N=vec.n 0<N<=MAX_C i 0<=pos<N i vec.t=T
//Post: s'ha eliminat T[pos] mantenint l'ordre i vec.n=N-1
    for(int i=pos;i<vec.n-1;i++){
        vec.t[i]=vec.t[i+1];
    }
    vec.n--;
}

void confirmar_assistencia(V_grups& grups,V_concerts& programats,V_concerts& confirmats){
//Pre: 0<=programats.n<=MAX_C i 0<=confirmats.n<=MAX_C i 0<=grups.n<=MAX_G i taules programats.t fins a programats.n-1 i programats.t[0..programats.n-1] ordeant per grup i data i hora d'inici i confirmats.t[0..confirmats.n-1] per data i grups.t[0..grups.n-1] per grup
//Post: s'ha confirmat l'assistència a un concert llegit de teclat, les taules programats, confirmats i grups s'han modificat en consonància.
    Concert c=llegir_grup_i_inici();
    bool trobat;
    int pos;
    apareix_i_pos(confirmats,c,'D',trobat,pos);
    if(trobat) cout<<"Ja esta confirmat"<<endl;
    else{
        apareix_i_pos(programats,c,'G',trobat,pos);
        if(not trobat) cout<<"No s'ha trobat el concert"<<endl;
        else{
            char entrada='S';
            if(programats.t[pos].entrada=='S'){
                cout<<"Tens l'entrada (S/N)? ";
                cin>>entrada;
            }
            if(entrada=='S'){
                if(programats.n<MAX_C){
                    inserir(programats.t[pos],confirmats,'D');
                    if(programats.t[pos].interes){
                        int pos_grup;
                        apareix_i_pos(programats.t[pos].grup,grups,trobat,pos_grup);
                        grups.t[pos_grup].n_anat++;
                    }
                    elimina_concert(programats,pos);
                    cout<<"Assistencia confirmada"<<endl;
                }
            }
            else cout<<"Per confirmar l'assistencia cal tenir entrada"<<endl;
        }
    }
}
void descompte_concert(V_grups& grups, string grup, char tipus_concert){
//Pre: 0<grups.n<=MAX_G
//Post: s'ha descomptat un conert a les estadístiques de grups
    bool trobat;
    int pos;
    apareix_i_pos(grup,grups,trobat,pos);
    if(trobat){
        grups.t[pos].n_fets--;
        if(tipus_concert=='C') grups.t[pos].n_anat--;
    }
}

void cancelar_concert(V_grups& grups,V_concerts& programats, V_concerts& confirmats){
//Pre: 0<=programats.n<=MAX_C i 0<=confirmats.n<=MAX_C i 0<=grups.n<=MAX_G i programats.t[0..programats.n-1] ordenats per grup i els confirmats per data i hora d'inici i confirmats.t[0..confirmats.n-1] prdenats per data i grups.t[0..grups.n-1] per grup
//Post: s'ha cancelat el concert indicat per teclat i s'han acutalitzat les taules programats o confirmats eliminant-lo mantenint l'ordre i les estadístiques del seu grup a grups.t
    Concert c=llegir_grup_i_inici();
    bool trobat;
    int pos;
    apareix_i_pos(confirmats,c,'D',trobat,pos);
    if(trobat){
        cout<<"Es cancela un concert amb assistencia confirmada"<<endl;
        if(confirmats.t[pos].entrada=='S') cout<<"Se't retornaran els diners de l'entrada"<<endl;
        if(confirmats.t[pos].interes) descompte_concert(grups,confirmats.t[pos].grup,'C');
        elimina_concert(confirmats, pos);
    }
    else{
        apareix_i_pos(programats,c,'G',trobat,pos);
        if(not trobat) cout<<"No s'ha trobat el concert"<<endl;
        else{
            if(programats.t[pos].interes) descompte_concert(grups,programats.t[pos].grup,'P');
            elimina_concert(programats,pos);
            cout<<"Es cancela un concert programat"<<endl;
        }

    }
}



void eliminar_concerts_abans_dia(V_concerts&programats, V_concerts& confirmats, Dia_hora dia){
//Pre:0<=programats.n<=MAX_C i 0<=confirmats.n<=MAX_C i programats.t[0..programats.n-1] ordenats per grup i data i hora d'inici i confirmats.t[0..confirmats.n-1] ordenats per data
//Post:s'han eliminat tots els concerts de programats i confirmats anterior a dia i s'han actualitzat programats.n i confirmats.n en consonància
    //eliminar el concerts programats.t que està ordenada per grup

    int pos_c=0;
    for(int i=0; i<programats.n;i++){
       if(passa_abans(dia, programats.t[i].inici)){
          programats.t[pos_c]=programats.t[i];
          pos_c++;
       }
    }
    programats.n=pos_c;

    // eliminar els concerts de la taula confirmats.t que està ordenada per data
    int i=0;
    while(i<confirmats.n and passa_abans(confirmats.t[i].inici,dia))i++;
    int n=0;
    while(i<confirmats.n){
        confirmats.t[n]=confirmats.t[i];
        n++;
        i++;
    }
    confirmats.n=n;
}


void mostrar_fusio_ordenada_distingint(const V_concerts& programats,const V_concerts& confirmats, int pos_inicial, Dia_hora dia){
//Pre:0<=programats.n<=MAX_C i 0<=confirmats.n<=MAX_C i programats.t[0..programats.n-1]  ordenats per grup i data i hora d'inici i confirmats.t[0..confirmats.n-1] per data
//Post: es mostren tots els concerts del dia indicant si provenen de la taula de programats o confirmats.
//mostrem la fusió de vec1 i confirmats
    int i1=0,i2=pos_inicial;
    while(i1<programats.n and i2<confirmats.n and mateix_dia(confirmats.t[i2].inici,dia)){
        if(es_menor(programats.t[i1], confirmats.t[i2],'D')){
            cout<<' ';
            mostrar_concert(programats.t[i1]);
            i1++;
        }
        else{
            cout<<'C';
            mostrar_concert(confirmats.t[i2]);
            i2++;
        }

    }

    while(i1<programats.n){
        cout<<' ';
        mostrar_concert(programats.t[i1]);
        i1++;
    }

    while(i2<confirmats.n and mateix_dia(confirmats.t[i2].inici,dia)){
        cout<<'C';
        mostrar_concert(confirmats.t[i2]);
        i2++;
    }

}

void intercanvi(Concert& a, Concert&b){
//Pre: a =A i b=B
//post: a=B i b=A
    Concert aux=a;
    a=b;
    b=aux;
}

void ordenar(V_concerts &vec, char criteri){
//Pre: 0<=vec.n<=MAX_C
//post: mostra vec.t[0..vec.n-1] ordeant per criteri

    for(int i=0; i<vec.n-1;i++){
        int pos_min=i;
        for(int j=i+1;j<vec.n;j++)
            if(es_menor(vec.t[j],vec.t[pos_min],criteri)) pos_min=j;
        intercanvi(vec.t[i],vec.t[pos_min]);
    }
}

void mostrar_concerts_dia(const V_concerts& programats,const V_concerts& confirmats, Dia_hora dia){
//Pre:0<=programats.n<=MAX_C i 0<=confirmats.n<=MAX_C i programats.t[0..programats.n-1] ordenats per grup i data i hora d'inici i confirmats.t[0..confirmats.n-1] per data
//Post: s'han mostrat tots els concets del dia dia tant els de programats.t[0..programats.n-1] com els de confirmats.t[0..confirmats.n-1]
    V_concerts programats_dia;
    //obtenim els concerts programats del dia
    programats_dia.n=0;
    for(int i=0;i<programats.n;i++){
        if(mateix_dia(dia,programats.t[i].inici)){
            programats_dia.t[programats_dia.n]=programats.t[i];
            programats_dia.n++;
        }
    }
    ordenar(programats_dia,'D');
    //obtenim la posició del primer concert confirmat d'aquest dia
    int i=0;
    while(i<confirmats.n and passa_abans(confirmats.t[i].inici,dia)) i++;

    //fem la fusió de les dues taules
    mostrar_fusio_ordenada_distingint(programats_dia,confirmats,i,dia);
}


char llegir_opcio(){
//Pre: cert
//Post: retorna l'opció llegida de teclat
    cout<<"Opcio (H: Menu): ";
    char c;
    cin>>c;
    return c;
}

void mostrar_menu(){
//Pre: cet
//Post: mostra el menú per ptantalla
    cout<<setw(20)<<setfill('*')<<" Menu "<<setw(10)<<setfill('*')<<"*"<<endl;
    cout<<"P: Eliminar concerts passats"<<endl;
    cout<<"N: Afegir noves programacions"<<endl;
    cout<<"C: Cancel.lar un concert"<<endl;
    cout<<"A: Confirmar assistencia a un concert"<<endl;
    cout<<"M: Mostrar els concerts"<<endl;
    cout<<"E: Mostrar els concerts confirmats d'un esdeveniment"<<endl;
    cout<<"D: On anar un dia"<<endl;
    cout<<"G: Estadistiques dels grups d'interes"<<endl;
    cout<<"I: Actualitzar grup d'interes"<<endl;
    cout<<"H: Ajuda "<<endl;
    cout<<"S: Sortir"<<endl;
    cout<<setw(30)<<setfill('*')<<"*"<<endl;
};

void mostrar_ordenat(V_concerts vec,char op, char criteri){
//Pre: 0<=vec.n<=MAX_C
//post: mostra vec.t[0..vec.n-1] ordeant per criteri

    ordenar(vec,criteri);
    mostrar_concerts(vec,op);

}

void mostrar_concerts_esdeveniment(const V_concerts vec, string esdev){
//Pre: 0<=vec.n<=MAX_C
//Post: ha mostrat vec.t[0..vec.n-1] per pantalla
    for(int i=0; i< vec.n;i++)
        if(vec.t[i].esdev==esdev) mostrar_concert(vec.t[i]);
}

int main()
{
    V_grups grups;
    omplir_vector_grups(grups);

    V_concerts confirmats;
    confirmats.n=0;

    V_concerts programats;
    omplir_vector_concerts(grups, programats);

    mostrar_menu();
    char opcio = llegir_opcio();
    while(opcio!='S'){
        if(opcio=='P'){
            cout<<"Data: ";
            Dia_hora dia=llegir_dia();
            eliminar_concerts_abans_dia(programats,confirmats,dia);
        }
        else if(opcio=='N') afegir_noves_programacions(programats, grups);
        else if(opcio=='C') cancelar_concert(grups,programats,confirmats);//NO??
        else if(opcio=='A') confirmar_assistencia(grups,programats,confirmats);
        else if(opcio=='M'){
            char tipus_concerts, ordre;
            cout<<"Introdueix tipus de concerts (C/P) i criteri d'ordenacio (G/E/D/L): ";
            cin>>tipus_concerts>>ordre;
            if(tipus_concerts=='C' and ordre =='D') mostrar_concerts(confirmats,'C');
            else if(tipus_concerts=='P' and ordre=='G') mostrar_concerts(programats,'P');
            else if(tipus_concerts=='C') mostrar_ordenat(confirmats,tipus_concerts, ordre);
            else mostrar_ordenat(programats,tipus_concerts, ordre);
        }
        else if(opcio=='E'){
            cout<<"Esdeveniment a cercar: ";
            string esdeveniment;
            cin>>esdeveniment;
            mostrar_concerts_esdeveniment(confirmats, esdeveniment);
        }
        else if(opcio=='D'){
            cout<<"Data: ";
            Dia_hora dia=llegir_dia();
            mostrar_concerts_dia(programats,confirmats,dia);
        }
        else if(opcio=='G') mostrar_estadistiques_grups(grups);
        else if (opcio=='I'){
            cout<<"Grup a actualitzar: ";
            string grup; cin>>grup;
            actualitzar_grups(grup,grups,programats,confirmats);
        }
        else if(opcio=='H') mostrar_menu();
        opcio=llegir_opcio();
    }

}
