#include <stdio.h>
#include <math.h>
#include <sys/stat.h>
#include <sys/types.h>

using namespace std;

void calMagneticField( string dirname ) {

   // make working directory for magnetic field
   string inputroot = dirname + "/inPair.root";
   string workdir = dirname + "/magnetic_field";
   const char* inroot = inputroot.c_str();
   const char* d = workdir.c_str();

   struct stat statBuf;

   if( stat(d, &statBuf) != 0 )
      mkdir(d, 0755);
   
   // define calculation variable
   int cal_k, origin_k;
   double cal_weight, origin_weight;
   double cal_x, cal_y, cal_z, cal_px, cal_py, cal_pz;
   double origin_x, origin_y, origin_z, origin_px, origin_py, origin_pz;
   double momentum_r, gamma, omega;
   double cal_r, cal_pt, cal_time;
   double cal_vx, cal_vy;
   double cal_ptmf;

   // define distance until hit distribution
   double distance;
   double magnetic_field;
   
   // define constant
   // const double magnetic_field = 3.5; // T
   const double CMENERGY = 250E+9; // eV
   const double EMASS = 9.10938E-31; // kg
   const double SPEED_OF_LIGHT = 2.99792E+8; // m/s
   const double ERESTENERGY = 5.10994E+5; // eV
   const double CONVERSION_4M = 5.34427E-28; // kg m/s c/eV
   
   // define output root file
   string rootfile = workdir + "/inPairOnMF.root";
   const char* rt = rootfile.c_str();

   TFile* tf = new TFile(rt, "RECREATE");
   TTree* tt = new TTree("t", "tree");

   tt->Branch("k", &cal_k, "k/I");
   tt->Branch("weight", &cal_weight, "weight/D");
   tt->Branch("x", &cal_x, "x/D");
   tt->Branch("y", &cal_y, "y/D");
   tt->Branch("z", &cal_z, "z/D");
   tt->Branch("px", &cal_px, "px/D");
   tt->Branch("py", &cal_py, "py/D");
   tt->Branch("pz", &cal_pz, "pz/D");
   tt->Branch("r", &cal_r, "r/D");
   tt->Branch("pt", &cal_pt, "pt/D");
   tt->Branch("time", &cal_time, "time/D");
   tt->Branch("vx", &cal_vx, "vx/D");
   tt->Branch("vy", &cal_vy, "vy/D");
   tt->Branch("pt_inMF", &cal_ptmf, "pt_inMF/D");
   

   // read input root file
   TFile* fin = new TFile( inroot, "read" );
   TTree* tin = (TTree*)fin->Get( "in_1000" );

   tin->SetBranchAddress("k", &origin_k);
   tin->SetBranchAddress("weight", &origin_weight);
   tin->SetBranchAddress("x", &origin_x);
   tin->SetBranchAddress("y", &origin_y);
   tin->SetBranchAddress("s", &origin_z);
   tin->SetBranchAddress("px", &origin_px);
   tin->SetBranchAddress("py", &origin_py);
   tin->SetBranchAddress("ps", &origin_pz);

   // check 
   cout << "----------Setting----------" << endl;
   cout << "workdirectory : " << workdir << endl;
   cout << "input root file : " << inputroot << endl;
   cout << "output root file : " << rootfile << endl;

   double pt, theta, vc, vz, time, charge, coefficient;

   unsigned int n = tin->GetEntries();
   cout << "GetEntries -> " << n << endl;
   for( int i = 0; i < n; i++ ) {
      tin->GetEntry(i);

      momentum_r = sqrt( pow( origin_px, 2.0 ) + pow(origin_py, 2.0 ) + pow( origin_pz, 2.0 ));
      gamma = momentum_r / ERESTENERGY + 1.0;

      // cal thate from the tangent of pt nad pz
      pt = sqrt( pow( origin_px, 2.0 ) + pow(origin_py, 2.0 ));
      theta = atan( pt/origin_pz );
      // cal velocity from the ration of spped_of_light
      vc = sqrt( 1.0 - 1.0 / pow( gamma, 2.0 ) );
      vc *= SPEED_OF_LIGHT;

      // cal vz
      vz = vc * cos( theta );

      if( origin_pz > 0.0 ) {

         distance = 4.0;
         magnetic_field = 3.5;
         time = ( distance - origin_z*1.0E-6 ) / vz;

         if( origin_k == 2 )		charge = -1.60217E-19;
         else if( origin_k == 3 )	charge = 1.60217E-19;

      }
      else {
         
         distance = -4.0;
         magnetic_field = 3.5;
         time = ( distance - origin_z*1.0E-6 ) / vz;

         if( origin_k == 2 )		charge = -1.60217E-19;

         else if( origin_k == 3 )	charge = 1.60217E-19;
      }

      omega = charge * magnetic_field / ( gamma * EMASS );
      
      double x1 = origin_px*CONVERSION_4M / ( charge * magnetic_field ) * sin( omega * time );
      double x2 = origin_py*CONVERSION_4M / ( charge * magnetic_field ) * cos( omega * time );
      origin_x *= 1.0E-6;
      cal_x = (x1 - x2 + origin_x + origin_py*CONVERSION_4M / ( charge * magnetic_field ) )*1.0E+3;

      double y1 = origin_px*CONVERSION_4M / ( charge * magnetic_field ) * cos( omega * time );
      double y2 = origin_py*CONVERSION_4M / ( charge * magnetic_field ) * sin( omega * time );
      origin_y *= 1.0E-6; // unit micron->m
      cal_y = (y1 + y2 + origin_y - origin_px*CONVERSION_4M / ( charge * magnetic_field ))*1.0E+3; // unit m->mm

      cal_z = ( vz * time + origin_z*1.0E-6 ) * 1.0E+3;

      cal_vx = origin_px*CONVERSION_4M / ( gamma * EMASS ) * cos( omega * time ) + origin_py*CONVERSION_4M / ( gamma * EMASS ) * sin( omega * time );
      cal_vy = -1.0 * origin_px*CONVERSION_4M / ( gamma * EMASS ) * sin( omega * time ) + origin_py*CONVERSION_4M / ( gamma * EMASS ) * cos( omega * time );

      cal_px = origin_px;
      cal_py = origin_py;
      cal_pz = origin_pz;

      cal_k = origin_k;

      cal_weight = origin_weight;

      cal_pt = pt;
      cal_ptmf = sqrt( pow( cal_vx*gamma*EMASS, 2.0 ) + pow( cal_vy*gamma*EMASS, 2.0 )) / CONVERSION_4M;

      cal_time = -1.0 * time / M_PI * omega;

      cal_r = abs( sqrt( origin_px*CONVERSION_4M * origin_px*CONVERSION_4M + origin_py*CONVERSION_4M * origin_py*CONVERSION_4M ) / charge / magnetic_field );

      if( i % 10000 == 0 ) {
         cout << "-----------------------------------------------------------------------------------------------" << endl;
         cout << "k = " << cal_k << "\t\t\t\t" << "time = " << time << " (s),\t"
                   << "v/c = " << vc / SPEED_OF_LIGHT << endl;
         cout << "px = " << scientific << origin_px << " (eV/c),\t"
                   << "py = " << scientific << origin_py << " (eV/c),\t"
                   << "pz = " << scientific << origin_pz << " (eV/c)" << endl;
         cout << "cal_x = " << scientific << cal_x << " (mm),\t"
                   << "cal_y = " << scientific << cal_y << " (mm),\t"
                   << "cal_z = " << scientific << cal_z << " (mm)" << endl;
      }

      tt->Fill();
      
   }

   tf->cd();
   tt->Write();
   tf->Close();
}
