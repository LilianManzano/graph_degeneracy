#include<iostream>
#include<cstdlib>
#include <string>
#include <vector>
#include <fstream>
#include <cmath>

using namespace std;

//permet de copier un graphe dans un tableau avec une colonne "marquage" en plus
void copierGraphe(int nbAretes, int aretes[][2], int copieAretes[][3]){
    for(int i=0;i<nbAretes;i++)
    {
        copieAretes[i][0]=aretes[i][0];
        copieAretes[i][1]=aretes[i][1];
        copieAretes[i][2]=0;
    }
}


//permet de vérifier la présence d'un entier dans un vector
bool present(int n, vector<int> v){
    bool present=false;
    for(int i=0;i<v.size();i++)
        {
        if(v[i]==n)
            present=true;
        }
    return present;
}


//permet d'obtenir le nombre de sommets d'un graphe
int getNbSommets(int nbAretes, int aretes[][2]){
    vector<int> sommets;
    for(int i=0;i<nbAretes;i++)
    {
        if(!present(aretes[i][0], sommets))
           sommets.push_back(aretes[i][0]);
        if(!present(aretes[i][1], sommets))
           sommets.push_back(aretes[i][1]);
    }
    return sommets.size();
}

//permet de remplir un tableau avec les sommet d'un graphe, prevoir une colonne pour leur degré, et une colonne pour le "marquage"
void remplirTabSommets(int nbAretes, int aretes[][2], int nbSommet, int sommets[][3]){
    vector<int> listeSommets;
    for(int i=0;i<nbAretes;i++)
    {
        if(!present(aretes[i][0], listeSommets))
           listeSommets.push_back(aretes[i][0]);
        if(!present(aretes[i][1], listeSommets))
           listeSommets.push_back(aretes[i][1]);
    }
    for(int i=0;i<nbSommet;i++)
    {
        sommets[i][0]=listeSommets[i];
        sommets[i][1]=0;
        sommets[i][2]=0;
    }
}

//permet de remplir la colonne "degre" du tableau de sommet à partir du tableau de graphe avec "marquage"
void calculDegre(int nbAretes, int aretes[][3], int nbSommet, int sommets[][3]){
    int degre;
    for(int i=0;i<nbSommet;i++)
    {
        if(sommets[i][2]==0)
        {
            degre=0;
            for(int j=0;j<nbAretes;j++)
            {
                if(aretes[j][0]==sommets[i][0]&&aretes[j][2]==0)
                    degre++;
                if(aretes[j][1]==sommets[i][0]&&aretes[j][2]==0)
                    degre++;
            }
            sommets[i][1]=degre;
        }
    }
}

//permet de marquer toutes les lignes contenant le sommet en paramêtre
void marquerSommet(int nbAretes, int aretes[][3], int sommet){
    for(int i=0;i<nbAretes;i++)
    {
        if(aretes[i][0]==sommet||aretes[i][1]==sommet)
           aretes[i][2]=1;
    }
}

//permet de calculer la degenerescence
int calculDege(int nbAretes, int aretes[][2], int nbSommets, int centres[][2]){

    int copieAretes[nbAretes][3];
    copierGraphe(nbAretes, aretes, copieAretes);//je copie le graphe dans un tableau me permettant de marquer les arêtes qui seront supprimés

    int sommets[nbSommets][3];
    remplirTabSommets(nbAretes, aretes, nbSommets, sommets);//je remplie un tableau avec les sommet, leur degré et prevoit une colonne pour marquer les sommet supprimés

    int k=0;
    int nbNonMarques=nbSommets;
    int nbSommetSup=0;

    //tant qu'il reste des sommets
    while(nbNonMarques>0)
    {
        k++;
        do //tant qu'il y a un sommet de supprimé(les degré ont potentiellement changé)
        {
            nbSommetSup=0;
            calculDegre(nbAretes, copieAretes, nbSommets, sommets);//on calcul les degres

            for(int i=0;i<nbSommets;i++)//on parcours les sommets
            {
                if (sommets[i][2]==0 && sommets[i][1]<=k)//si le sommet n'est pas marqué et qu'il est de degre inferieur ou egal à k
                {
                    printf("sommet supprime: %d avec k=%d\n",sommets[i][0],k);
                    centres[i][0]=sommets[i][0];
                    centres[i][1]=k;
                    int sommetSup=sommets[i][0];
                    sommets[i][2]=1;//on marque le sommet comme supprimé
                    marquerSommet(nbAretes, copieAretes, sommetSup);//on marque toutes les lignes du tableau du graphe où il y a le sommet
					nbSommetSup++;//on indique qu'il y a eu un sommet de plus de supprimé (ce qui a potentiellement changé le degré d'autres sommets)
                }
            }
            nbNonMarques=nbNonMarques-nbSommetSup;
        }while (nbSommetSup>0);
    }
    return k;
}

//permet d'afficher les centres de chaque sommet
void afficherCentres(int nbSommets, int centres[][2]){
    printf("Sommet: ");
    for(int i=0;i<nbSommets;i++)
    {
        printf("%d ",centres[i][0]);
    }
    printf("\nCentre: ");
    for(int i=0;i<nbSommets;i++)
    {
        printf("%d ",centres[i][1]);
    }
    printf("\n");
}

//permet d'afficher les aretes (avec noeuds 2 par 2)
void afficherAretes(int nbAretes, int aretes[][2]){
    printf("aretes: \n");
    for(int i=0;i<nbAretes;i++)
    {
        printf("%d %d \n",aretes[i][0],aretes[i][1]);
    }
    printf("\n");
}


//permet de récupérer dans un vector les lignes comportant les aretes à partir d'un fichier (ne récupère pas les lignes ne commençant pas par un chiffre donc ne récupère pas les lignes de commentaires et lignes vides)
vector<string> importListeAretes(string cheminFichier){
    string ligne;
    vector<string> lignes;
    ifstream fichier(cheminFichier);
    if(!fichier.is_open()){
        cerr << "Le fichier suivant '"<<cheminFichier<<"' n'a pas pu etre ouvert"<< endl;
    }
    else{
        while(getline(fichier, ligne))//on récupère d'abord toutes les lignes
        {
            lignes.push_back(ligne);
        }

        for(int i=0;i<lignes.size();i++)//puis on vérifie que le debut de chaque ligne commence par un chiffre sinon on la supprime
        {
            ligne=lignes[i];
            if (ligne[0]!='0' && ligne[0]!='1' && ligne[0]!='2' && ligne[0]!='3' && ligne[0]!='4' && ligne[0]!='5' && ligne[0]!='6' && ligne[0]!='7' && ligne[0]!='8' && ligne[0]!='9')
            {
                lignes.erase(lignes.begin()+i);
                i--;
            }
        }
    }
    fichier.close();
    return lignes;
}

//permet d'importer dans un tableau les aretes (sommets 2 par 2) à partir d'une liste de string(nécésité d'avoir uniquement des lignes avec 2 nombres)
void importGraphe(int nbAretes, int aretes[][2], vector<string> lignes){
    string ligne;
    for(int i=0;i<nbAretes;i++){
        ligne=lignes[i];

        int debutNombre1=0;
        int finNombre1=0;
        while(ligne[finNombre1+1]=='0' || ligne[finNombre1+1]=='1' || ligne[finNombre1+1]=='2' || ligne[finNombre1+1]=='3' || ligne[finNombre1+1]=='4' || ligne[finNombre1+1]=='5' || ligne[finNombre1+1]=='6' || ligne[finNombre1+1]=='7' || ligne[finNombre1+1]=='8' || ligne[finNombre1+1]=='9')
            {finNombre1++;}
        int debutNombre2=finNombre1+1;
        while(ligne[debutNombre2]!='0' && ligne[debutNombre2]!='1' && ligne[debutNombre2]!='2' && ligne[debutNombre2]!='3' && ligne[debutNombre2]!='4' && ligne[debutNombre2]!='5' && ligne[debutNombre2]!='6' && ligne[debutNombre2]!='7' && ligne[debutNombre2]!='8' && ligne[debutNombre2]!='9')
            {debutNombre2++;}
        int finNombre2=ligne.size()-1;

        int tailleNombre1=finNombre1-debutNombre1+1;
        int tailleNombre2=finNombre2-debutNombre2+1;

        string nb1;
        nb1=ligne.substr(debutNombre1,tailleNombre1);
        string nb2;
        nb2=ligne.substr(debutNombre2,tailleNombre2);

        aretes[i][0]=stoi(nb1);
        aretes[i][1]=stoi(nb2);
    }
}


//permet de trier le tableau des centres du plus petit au plus grand
void trierCentres(int nbSommets, int centres[][2], int degenerescence){
    for(int i=0; i<nbSommets; i++)
    {
        int minimum=degenerescence;
        int indice=-1;
        for (int j=i; j<nbSommets; j++)
        {

            if(centres[j][1]==minimum && centres[j][0]<centres[i][0])
            {
                indice=j;
            }
            if(centres[j][1]==minimum && indice== -1)
            {
                indice=i;
            }
            if(centres[j][1]<minimum)
            {
                minimum=centres[j][1];
                indice=j;
            }
        }
    int tempSommet=centres[i][0];
    int tempValeur=centres[i][1];
    centres[i][0]=centres[indice][0];
    centres[i][1]=centres[indice][1];
    centres[indice][0]=tempSommet;
    centres[indice][1]=tempValeur;
    }
}


//permet de dessiner le graphe avec les plus gros centres à l'intérieur d'un cercle, et les plus faibles à l'extérieur
void dessinGraphe(int nbAretes, int aretes[][2], int nbSommets, int centres[][2], int degenerescence){

    FILE *outfile;
    outfile = fopen("./drawing.ps", "w");

    trierCentres(nbSommets,centres,degenerescence);

    int nbSommetsParCentres[degenerescence];
    for(int i=0;i<degenerescence;i++)
    {
        int nb=0;
        for(int j=0;j<nbSommets;j++)
        {
            if(centres[j][1]==i+1)
            {
                nb++;
            }
        }
        nbSommetsParCentres[i]=nb;
    }


    double coordSommet[nbSommets][4];
    int rayonCercle=295/degenerescence;
    int indice=0;
    double pi = acos(-1);
    for(int i=0;i<degenerescence;i++)
    {
        int angle=360/nbSommetsParCentres[i];
        for(int j=0;j<nbSommetsParCentres[i];j++)
        {
            coordSommet[indice][0]=centres[indice][0];
            coordSommet[indice][1]=centres[indice][1];
            coordSommet[indice][2]=306+((degenerescence-coordSommet[indice][1]+1)*rayonCercle)*cos(((double)j*(double)angle)/180*pi);
            coordSommet[indice][3]=391+((degenerescence-coordSommet[indice][1]+1)*rayonCercle)*sin(((double)j*(double)angle)/180*pi);
            indice++;
        }
    }

    // dessine le point central et les cercles rouges
    fprintf(outfile,"%%!\n306 391 2.5 0 360 arc\nfill\n255 0 0 setrgbcolor\n");
    for(int i=1;i<=degenerescence;i++){
        int rayon=rayonCercle*i;
        fprintf(outfile,"%d 391 moveto\n306 391 %d 0 360 arc\n",306+rayon,rayon);
    }
    fprintf(outfile,"stroke\n0 0 0 setrgbcolor\n");
    //dessine les sommets
    for(int i=0;i<nbSommets;i++){
        int x=coordSommet[i][2];
        int y=coordSommet[i][3];
        fprintf(outfile,"%d %d moveto\n%d %d 10 0 360 arc\nfill\n",x,y,x,y);
    }
    fprintf(outfile,"fill\nstroke\n");
    //dessine les arretes
    for(int i=0; i<nbAretes;i++){
        int ind1=0;int ind2=0;
        while(aretes[i][0]!=coordSommet[ind1][0]){
            ind1++;
        }
        while(aretes[i][1]!=coordSommet[ind2][0]){
            ind2++;
        }
        int x1=coordSommet[ind1][2];
        int y1=coordSommet[ind1][3];
        int x2=coordSommet[ind2][2];
        int y2=coordSommet[ind2][3];
        fprintf(outfile,"%d %d moveto\n%d %d lineto\n",x1,y1,x2,y2);
    }
    fprintf(outfile,"stroke\n");
    //dessine les numero de sommets
    fprintf(outfile,"255 255 255 setrgbcolor\n/Helvetica-Bold findfont\n10 scalefont\nsetfont\n");
    for(int i=0;i<nbSommets;i++){
        int x=coordSommet[i][2]-5;
        int y=coordSommet[i][3]-3;
        int s=coordSommet[i][0];
        fprintf(outfile,"%d %d moveto\n(%d) show\n",x,y,s);
    }
    fprintf(outfile,"stroke");

    fclose(outfile);
}




//----------------------MAIN-------------------------------

int main(){
    //on note un tableau les arêtes (sommets 2 par 2) récupérées dans un fichier dont le lien a été donné
    string cheminFichier;
    cout << "Entrer le chemin du fichier de donnees :"<< endl;
    cin >> cheminFichier;

    vector<string> listeAretes=importListeAretes(cheminFichier);
    int nbAretes=listeAretes.size();
    int aretes[nbAretes][2];
    importGraphe(nbAretes,aretes,listeAretes);

    //on calcul la dégénérescence et on note les centres par la même occasion
    int nbSommets=getNbSommets(nbAretes, aretes);
    int centres[nbSommets][2];
    int degenerescence=calculDege(nbAretes, aretes, nbSommets, centres);

    printf("\nLa degenerescence du graphe est de %d\n",degenerescence);
    printf("\nTableau des centres :\n");
    afficherCentres(nbSommets, centres);

    //on dessine le graphe dans un fichier Postscript "dessin.ps"
    dessinGraphe(nbAretes,aretes,nbSommets,centres,degenerescence);



//fonctionne uniquement sous linux (besoin d'installer des librairies ou utiliser des APIs sur autres environnements)
//-----------------------------------------------------------------
printf("\nLe fichier .ps tente d'etre converti en .pdf si la commande 'ps2pdf' est disponible sur votre machine\n");
system ("ps2pdf ./drawing.ps ./drawing.pdf");
//-----------------------------------------------------------------
}
