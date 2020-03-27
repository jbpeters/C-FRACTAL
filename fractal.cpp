//:Fractals : A single program to produce Mandelbrot and Julia sets
//      using Gnu's Plotlib Plotter class and outputing to an X-window or a
//      PNG file.  uses a pallette array somewhat faster but seg-faults for highter iteration values.
//Author:Jeff Peters
//Date: September 2006 ~ April 2007
//
//to compile: g++ -lplotter -lXaw -lXmu -lXt -lSM -lICE -lXext -lX11 -lm  -ofractal fractal.cpp
//
//Date: February 2013
//     built on code::blocks 10.05 IDE
//
//Date August 2015
//     simplified pallet formula  w = _c*pi/max_itr  
//                                d = w(k)*max_itr+pi  
//     now is same as python palette see "pyfractal"
//
#define INCLUDE_PNG_SUPPORT 1
#include<iostream>
#include<cmath>
#include<plotter.h>
#include<cstdlib>
#include<fstream>
using namespace std;


//Function Decl.
int Usage(char * name);

//Function Defn.
int Usage(int count , char * name) {
    cout<<"  usage: "<<name<<" ScreenSize[widthxheight]  plot-type[ X | P ] "<<"Set-type[ M | J ] "<<"x y ip.real ip.imag radius  _red _green _blue  #iterations"<<endl;
    return 1;
}//end.Usage ;

int main ( int argc , char * argv[]) {

// arg usage:
    if( argc != 13 )    {
        Usage( argc, argv[0]) ;
        return 1;
    }//if.end

    string screen        =      argv[1]  ;   // "300x400"
    char * plot          =      argv[2]  ;  // X , P
    char * type          =      argv[3]  ;  // M , J
    long double cx       = atof(argv[4]) ;
    long double cy       = atof(argv[5]) ;
    double ip_r          = atof(argv[6]) ;  // real iteration parameter; is 0 for M'brot
    double ip_i          = atof(argv[7]) ;  // imag iteration parameter; is 0 for M'brot
    long double r        = atof(argv[8]) ;  //radius
    double _r            = atof(argv[9]) ;  // red parameter
    double _g            = atof(argv[10]);  // green parameter
    double _b            = atof(argv[11]);  // blue parameter
    long long unsigned max_itr  = atoi(argv[12]);  // max iterations
    size_t len           =      screen.length() ;
    size_t p             =      screen.find("x");
    string width         =      screen.substr(0,p);
    string height        =      screen.substr(p+1,len+1-p);
    float x_screen       =      atof (width.c_str());
    float y_screen       =      atof (height.c_str());
    string pic_name;

    if ( r <= 0.0 )    {
        cerr<< "Negative radius!!"<< endl;
        return 1;
    }
    long double a,b;
    if (x_screen>y_screen)    {
        b=r;
        a=r*x_screen/y_screen;
    }
    else if (y_screen>x_screen)    {
        a=r;
        b=r*y_screen/x_screen;
        string picname;
    }  else     {
        a =  b = r;
    }
    double fudge;
    if ( strcmp(plot ,"X") == 0 )    {
        fudge=1.0;
    } else {
        fudge=1.0;
    }
    double x_gauge = 2*fudge*a/x_screen;
    double y_gauge = 2*fudge*b/y_screen;

// Plot config

    PlotterParams params;
    params.setplparam((const char*) "BITMAPSIZE",(char*) screen.c_str() );

//generate a meaningful name for the plot file
    Plotter  *graphic;

    ofstream somefile;
    if ( !strcmp( plot, "P")  )    {
        string spc = "_";
        string png = ".png";
        pic_name  = type + spc;
        pic_name += argv[4]  + spc ;
        pic_name += argv[5]  + spc ;
        pic_name += argv[6]  + spc ;
        pic_name += argv[7]  + spc ;
        pic_name += argv[8]  + spc ;
        pic_name += argv[9]  + spc ;
        pic_name += argv[10] + spc ;
        pic_name += argv[11] + spc ;
        pic_name += argv[12] + spc ;
        pic_name += png ;

        cout<<pic_name<<endl;


        somefile.open( pic_name.c_str() , ios::out );

        graphic = new   PNGPlotter(cin, somefile, cerr, params );
        if ( graphic->openpl() < 0)         {
            cerr<<"Couldn't open Plotter"<<endl;
            return 1;
        }
    }

    if ( !strcmp( plot, "X" ) )    {
        graphic = new XPlotter(cin, cout, cerr, params) ;
        if ( graphic->openpl() < 0)         {
            cerr<<"Couldn't open Plotter"<<endl;
            return 1;
        }
    }
// cout<<"pallette here!"<<endl;
// Pallette
    long int      vmax = 65536 ;
    double     pi = 4.0*atan(1.0) ;

    double omega_r =  _r*pi/max_itr ;
    double omega_g =  _g*pi/max_itr ;
    double omega_b =  _b*pi/max_itr ;

    double delta_r =  omega_r*max_itr+pi ;
    double delta_g =  omega_g*max_itr+pi ;
    double delta_b =  omega_b*max_itr+pi ;
//  cout<<"Max_iterations :"<<max_itr<<endl;
    int   red[max_itr] ;
    int green[max_itr] ;
    int  blue[max_itr] ;
//  cout<<"after ARRAY"<<endl;
    for(long long unsigned int i=0; i<=max_itr; i++) {
        red[i]    = (long long int) (vmax/2.0 * ( 1 + cos( omega_r * i - delta_r )));
        green[i]  = (long long int) (vmax/2.0 * ( 1 + cos( omega_g * i - delta_g )));
        blue[i]   = (long long int) (vmax/2.0 * ( 1 + cos( omega_b * i - delta_b )));
    }
// Screen Scan
    graphic->fspace(cx-fudge*a,cy-fudge*b,cx+fudge*a,cy+fudge*b);
    graphic->fcircle(cx,cy,r);
    graphic->fcircle(cx,cy,r/10);
    for (double  row=cy-b; row<cy+b ; row += y_gauge ){ //cy-h/2 ;cy+h2/ ; row += y_guage
        for (double col=cx-a ; col< cx+a ; col += x_gauge ){
            double d = 0;
            double zr=0,zi=0,nzr=0,nzi=0;
            double cr=0,ci=0;
            if (!strcmp(type, "M")) { // mandelbrot
                zr = 0 ;   //real(z) = 0.0
                zi = 0 ;   //imag(z) = 0.0;
                cr = col ; //real(c) = col;
                ci = row ; //imag(c) = row;
            }
            else if ( !strcmp(type,"J")) { // julia
                zr = col; //real(z) = col ;
                zi = row; //imag(z) = row ;
                cr = ip_r; //c = ip;           //iteration parameter
                ci = ip_i;
            }
            long long unsigned int count=0;

            while ( d<4   && count< max_itr ){ 
                nzr = zr*zr - zi*zi+ cr;   //z = pow(z,2) + c;
                nzi = 2*zr*zi + ci;
                d = nzr*nzr+nzi*nzi;
                count++;
                zr=nzr;
                zi=nzi;
            }
            graphic->color( red[count],green[count],blue[count] );
            graphic->fpoint(col,row);
        }
    }
//close plotter
    if ( graphic->closepl() < 0 ){
        cerr<<"Coulden't close Plotter"<<endl;
        return 1;
    }
    somefile.close();
} //main.end
