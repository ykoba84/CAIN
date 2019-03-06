
using namespace std;

void position2d(string fn, const char* rootfiles) {

   int xmin = -60;
   int ymin = -60;
   int xmax = 60;
   int ymax = 60;

   std::cout << "Processing position2d for " << fn << "..." << std::endl;
  
   std::string pdffile = fn + "/magnetic_field/" + fn + ".pdf(";
   const char* pf = pdffile.c_str();
  
   // rootファイルを読み込み
   TFile* fin1 = new TFile(rootfiles, "read");

   // Canvas生成
   TCanvas* cvs = new TCanvas("cvs", "cvs", 1000, 500);

   cvs->Divide(2,1);

   // y軸のタイトルのオフセット
   gStyle->SetTitleOffset(1.3, "y");

   //rootファイルのなかのttreeファイルを読み込む
   TTree* tin1 = (TTree*)fin1->Get( "t" );

   cvs->cd(1);

   // 軸範囲指定
   // DrawFrame(xmin, ymin, xmax, ymax, "title;xAxis_title;yAxis_title")
   TH1F* frame2 = (TH1F*)cvs->DrawFrame(xmin, ymin, xmax, ymax, "hit dist. (-z, no cut); x[mm]; y[mm]");

   tin1->Draw("x:y", "k == 2 && pz < 0", "goff");
   const int nPoints = tin1->GetSelectedRows();
   TGraph *gre1 = new TGraph(nPoints, tin1->GetV1(), tin1->GetV2());
   gre1->SetMarkerColor(9);
   gre1->Draw("P");

   cvs->cd(2);

// 軸範囲指定
   // DrawFrame(xmin, ymin, xmax, ymax, "title;xAxis_title;yAxis_title")
   TH1F* frame = (TH1F*)cvs->DrawFrame(xmin, ymin, xmax, ymax, "hit dist. (+z, no cut); x[mm]; y[mm]");

   tin1->Draw("x:y", "k == 2 && pz > 0", "goff");
   const int nPoints2 = tin1->GetSelectedRows();
   TGraph *gre2 = new TGraph(nPoints2, tin1->GetV1(), tin1->GetV2());
   gre2->SetMarkerColor(9);
   gre2->Draw("P");

   cvs->Print(pf);

   cvs->cd(1);
   // 軸範囲指定
   // DrawFrame(xmin, ymin, xmax, ymax, "title;xAxis_title;yAxis_title")
   TH1F* frame3 = (TH1F*)cvs->DrawFrame(xmin, ymin, xmax, ymax, "hit dist. (-z, no cut); x[mm]; y[mm]");

   tin1->Draw("x:y", "k == 3 && pz < 0", "goff");
   const int nPoints3 = tin1->GetSelectedRows();
   TGraph *grp1 = new TGraph(nPoints3, tin1->GetV1(), tin1->GetV2());
   grp1->SetMarkerColor(46);
   grp1->Draw("P");

   cvs->cd(2);
   // 軸範囲指定
   // DrawFrame(xmin, ymin, xmax, ymax, "title;xAxis_title;yAxis_title")
   TH1F* frame4 = (TH1F*)cvs->DrawFrame(xmin, ymin, xmax, ymax, "hit dist. (+z, no cut); x[mm]; y[mm]");

   tin1->Draw("x:y", "k == 3 && pz > 0", "goff");
   const int nPoints4 = tin1->GetSelectedRows();
   TGraph *grp2 = new TGraph(nPoints4, tin1->GetV1(), tin1->GetV2());
   grp2->SetMarkerColor(46);
   grp2->Draw("P");

   cvs->Print(pf);
   cvs->Close();
}

void plot_phi(string fn, const char* rootfiles) {

   int k;
   double x, y, z, w;
   double px, py, pz;

   std::cout << "Processing plot_phi for " << fn << "..." << std::endl;

   std::string pdffile = fn + "/magnetic_field/" + fn + ".pdf";
   const char* pf = pdffile.c_str();
  
   // rootファイルを読み込み
   TFile* fin1 = new TFile(rootfiles, "read");
   TTree* tin1 = (TTree*)fin1->Get( "t" );

   tin1->SetBranchAddress("k", &k);
   tin1->SetBranchAddress("weight", &w);
   tin1->SetBranchAddress("x", &x);
   tin1->SetBranchAddress("y", &y);
   tin1->SetBranchAddress("z", &z);
   tin1->SetBranchAddress("px", &px);
   tin1->SetBranchAddress("py", &py);
   tin1->SetBranchAddress("pz", &pz);

   // Canvas生成
   TCanvas* cvs = new TCanvas("cvs", "cvs", 1000, 500);

   // y軸のタイトルのオフセット
   gStyle->SetTitleOffset(1.5, "y");

   cvs->Divide(2,1);
  
   cvs->cd(1);

   double bins = 30*M_PI;
   double min = -M_PI;
   double max = M_PI;
   int ent_min = 0;
   int ent_max = 3000; 

   TH1F* frame1 = (TH1F*)cvs->DrawFrame(min, ent_min, max, ent_max, "#varphi (-z, no cut); #varphi[rad]; Entries");
  
   TH1D* eh1 = new TH1D("eh1", "", bins, min, max);
   TH1D* ph1 = new TH1D("ph1", "", bins, min, max);
   TH1D* th1 = new TH1D("th1", "", bins, min, max);

   // 凡例
   TLegend* leg = new TLegend(0.1, 0.8, 0.3, 0.9);
  
   unsigned int n = tin1->GetEntries();
   for( unsigned int i = 0; i < n; i++ ) {

      tin1->GetEntry(i);

      if( pz < 0 ) {
      
         double phi = std::atan2(py, px);

         if( i % 5000 == 0 ) std::cout << "phi = " << phi <<  std::endl;

         if( k == 2 ) {	
            for(int j = 0; j < w; j++ ) eh1->Fill(phi);
         }
         else if( k == 3 ) {
            for( int j = 0; j < w; j++ ) ph1->Fill(phi);
         }
      
         for( int j = 0; j < w; j++ ) th1->Fill(phi);
      }
   }

   // Statistics box, 0:非表示 1:表示
   th1->SetStats(0);
   eh1->SetStats(0);
   ph1->SetStats(0);

   // color設定
   th1->SetLineColor(1);
   eh1->SetLineColor(9);
   ph1->SetLineColor(46);

   // plot
   th1->Draw("same");
   eh1->Draw("same");
   ph1->Draw("same");

   // 凡例出力
   leg->AddEntry(eh1, "Electron", "l");
   leg->AddEntry(ph1, "Positron", "l");
   leg->AddEntry(th1, "Total", "l");
   leg->Draw();

   cvs->cd(2);

   TH1F* frame2 = (TH1F*)cvs->DrawFrame(min, ent_min, max, ent_max, "#varphi (+z, no cut); #varphi[rad]; Entries");

   TH1D* eh2 = new TH1D("eh2", "", bins, min, max);
   TH1D* ph2 = new TH1D("ph2", "", bins, min, max);
   TH1D* th2 = new TH1D("th2", "", bins, min, max);

   // 凡例
   TLegend* leg2 = new TLegend(0.1, 0.8, 0.3, 0.9);
  
   unsigned int m = tin1->GetEntries();
   for( unsigned int i = 0; i < m; i++ ) {

      tin1->GetEntry(i);

      if( pz > 0 ) {
      
         double phi = std::atan2(py, px);

         if( i % 5000 == 0 ) std::cout << "phi = " << phi <<  std::endl;

         if( k == 2 ) {	
            for(int j = 0; j < w; j++ ) eh2->Fill(phi);
         }
         else if( k == 3 ) {
            for( int j = 0; j < w; j++ ) ph2->Fill(phi);
         }
      
         for( int j = 0; j < w; j++ ) th2->Fill(phi);
      }
   }

   // Statistics box, 0:非表示 1:表示
   th2->SetStats(0);
   eh2->SetStats(0);
   ph2->SetStats(0);

   // color設定
   th2->SetLineColor(15);
   eh2->SetLineColor(38);
   ph2->SetLineColor(46);

   // positronのhistgramを強調表示
   ph2->SetFillStyle(1001);
   ph2->SetFillColor(46);

   // plot
   th2->Draw("same");
   eh2->Draw("same");
   ph2->Draw("same");

   // 凡例出力
   leg2->AddEntry(eh2, "Electron", "l");
   leg2->AddEntry(ph2, "Positron", "l");
   leg2->AddEntry(th2, "Total", "l");
   leg2->Draw();

   cvs->Print(pf);
   cvs->Close();
}

void plot_phi_after_analysis(string fn, const char* rootfiles) {

   int k;
   double x, y, z, w;
   double px, py, pz;
   double vx, vy;

   std::cout << "Processing plot_phi for " << fn << "..." << std::endl;

   std::string pdffile = fn + "/magnetic_field/" + fn + ".pdf";
   const char* pf = pdffile.c_str();
  
   // rootファイルを読み込み
   TFile* fin1 = new TFile(rootfiles, "read");
   TTree* tin1 = (TTree*)fin1->Get( "t" );

   tin1->SetBranchAddress("k", &k);
   tin1->SetBranchAddress("weight", &w);
   tin1->SetBranchAddress("x", &x);
   tin1->SetBranchAddress("y", &y);
   tin1->SetBranchAddress("z", &z);
   tin1->SetBranchAddress("px", &px);
   tin1->SetBranchAddress("py", &py);
   tin1->SetBranchAddress("pz", &pz);
   tin1->SetBranchAddress("vx", &vx);
   tin1->SetBranchAddress("vy", &vy);

   // Canvas生成
   TCanvas* cvs = new TCanvas("cvs", "cvs", 1000, 500);

   // y軸のタイトルのオフセット
   gStyle->SetTitleOffset(1.5, "y");

   cvs->Divide(2,1);
  
   cvs->cd(1);

   double bins = 30*M_PI;
   double min = -M_PI;
   double max = M_PI;
   int ent_min = 0;
   int ent_max = 1000; 

   TH1F* frame1 = (TH1F*)cvs->DrawFrame(min, ent_min, max, ent_max, "#varphi (-4m); #varphi[rad]; Entries");
  
   TH1D* eh1 = new TH1D("eh1", "", bins, min, max);
   TH1D* ph1 = new TH1D("ph1", "", bins, min, max);
   TH1D* th1 = new TH1D("th1", "", bins, min, max);

   // 凡例
   TLegend* leg = new TLegend(0.1, 0.8, 0.3, 0.9);
  
   unsigned int n = tin1->GetEntries();
   for( unsigned int i = 0; i < n; i++ ) {

      tin1->GetEntry(i);

      if( pz < 0 ) {
      
         double phi = std::atan2(vy, vx);

         //if( i % 5000 == 0 ) std::cout << "phi = " << phi <<  std::endl;

         if( k == 2 ) {	
            for(int j = 0; j < w; j++ ) eh1->Fill(phi);
         }
         else if( k == 3 ) {
            for( int j = 0; j < w; j++ ) ph1->Fill(phi);
         }
      
         for( int j = 0; j < w; j++ ) th1->Fill(phi);
      }
   }

   // Statistics box, 0:非表示 1:表示
   th1->SetStats(0);
   eh1->SetStats(0);
   ph1->SetStats(0);

   // color設定
   th1->SetLineColor(1);
   eh1->SetLineColor(9);
   ph1->SetLineColor(46);

   // plot
   th1->Draw("same");
   eh1->Draw("same");
   ph1->Draw("same");

   // 凡例出力
   leg->AddEntry(eh1, "Electron", "l");
   leg->AddEntry(ph1, "Positron", "l");
   leg->AddEntry(th1, "Total", "l");
   leg->Draw();

   cvs->cd(2);

   TH1F* frame2 = (TH1F*)cvs->DrawFrame(min, ent_min, max, ent_max, "#varphi (+4m); #varphi[rad]; Entries");

   TH1D* eh2 = new TH1D("eh2", "", bins, min, max);
   TH1D* ph2 = new TH1D("ph2", "", bins, min, max);
   TH1D* th2 = new TH1D("th2", "", bins, min, max);

   // 凡例
   TLegend* leg2 = new TLegend(0.1, 0.8, 0.3, 0.9);
  
   unsigned int m = tin1->GetEntries();
   for( unsigned int i = 0; i < m; i++ ) {

      tin1->GetEntry(i);

      if( pz > 0 ) {
      
         double phi = std::atan2(vy, vx);

         //if( i % 5000 == 0 ) std::cout << "phi = " << phi <<  std::endl;

         if( k == 2 ) {	
            for(int j = 0; j < w; j++ ) eh2->Fill(phi);
         }
         else if( k == 3 ) {
            for( int j = 0; j < w; j++ ) ph2->Fill(phi);
         }
      
         for( int j = 0; j < w; j++ ) th2->Fill(phi);
      }
   }

   // Statistics box, 0:非表示 1:表示
   th2->SetStats(0);
   eh2->SetStats(0);
   ph2->SetStats(0);

   // color設定
   th2->SetLineColor(1);
   eh2->SetLineColor(9);
   ph2->SetLineColor(46);

   // plot
   th2->Draw("same");
   eh2->Draw("same");
   ph2->Draw("same");

   // 凡例出力
   leg2->AddEntry(eh2, "Electron", "l");
   leg2->AddEntry(ph2, "Positron", "l");
   leg2->AddEntry(th2, "Total", "l");
   leg2->Draw();

   cvs->Print(pf);
   cvs->Close();
}

void plot_phi_after_analysis_with_hole(string fn, const char* rootfiles) {

   int k;
   double x, y, z, w;
   double px, py, pz;
   double vx, vy;

   std::cout << "Processing plot_phi for " << fn << "..." << std::endl;

   std::string pdffile = fn + "/magnetic_field/" + fn + ".pdf";
   const char* pf = pdffile.c_str();
  
   // rootファイルを読み込み
   TFile* fin1 = new TFile(rootfiles, "read");
   TTree* tin1 = (TTree*)fin1->Get( "t" );

   tin1->SetBranchAddress("k", &k);
   tin1->SetBranchAddress("weight", &w);
   tin1->SetBranchAddress("x", &x);
   tin1->SetBranchAddress("y", &y);
   tin1->SetBranchAddress("z", &z);
   tin1->SetBranchAddress("px", &px);
   tin1->SetBranchAddress("py", &py);
   tin1->SetBranchAddress("pz", &pz);
   tin1->SetBranchAddress("vx", &vx);
   tin1->SetBranchAddress("vy", &vy);

   // Canvas生成
   TCanvas* cvs = new TCanvas("cvs", "cvs", 1000, 500);

   // y軸のタイトルのオフセット
   gStyle->SetTitleOffset(1.5, "y");

   cvs->Divide(2,1);
  
   cvs->cd(1);

   double bins = 30*M_PI;
   double min = -M_PI;
   double max = M_PI;
   int ent_min = 0;
   int ent_max = 120; 

   TH1F* frame1 = (TH1F*)cvs->DrawFrame(min, ent_min, max, ent_max, "#varphi with one hole (-4m); #varphi[rad]; Entries");
  
   TH1D* eh1 = new TH1D("eh1", "", bins, min, max);
   TH1D* ph1 = new TH1D("ph1", "", bins, min, max);
   TH1D* th1 = new TH1D("th1", "", bins, min, max);

   // 凡例
   TLegend* leg = new TLegend(0.1, 0.8, 0.3, 0.9);
  
   unsigned int n = tin1->GetEntries();
   for( unsigned int i = 0; i < n; i++ ) {

      tin1->GetEntry(i);

      if( pz < 0 ) {
      
         double phi = std::atan2(vy, vx);

         //if( i % 5000 == 0 ) std::cout << "phi = " << phi <<  std::endl;

         if( std::sqrt( x*x + y*y ) > 18.0 ) {
            if( k == 2 ) {	
               for(int j = 0; j < w; j++ ) eh1->Fill(phi);
            }
            else if( k == 3 ) {
               for( int j = 0; j < w; j++ ) ph1->Fill(phi);
            }

            for( int j = 0; j < w; j++ ) th1->Fill(phi);
         }
      }
   }

   // Statistics box, 0:非表示 1:表示
   th1->SetStats(0);
   eh1->SetStats(0);
   ph1->SetStats(0);

   // color設定
   th1->SetLineColor(1);
   eh1->SetLineColor(9);
   ph1->SetLineColor(46);

   // plot
   th1->Draw("same");
   eh1->Draw("same");
   ph1->Draw("same");

   // 凡例出力
   leg->AddEntry(eh1, "Electron", "l");
   leg->AddEntry(ph1, "Positron", "l");
   leg->AddEntry(th1, "Total", "l");
   leg->Draw();

   cvs->cd(2);

   TH1F* frame2 = (TH1F*)cvs->DrawFrame(min, ent_min, max, ent_max, "#varphi with one hole (+4m); #varphi[rad]; Entries");

   TH1D* eh2 = new TH1D("eh2", "", bins, min, max);
   TH1D* ph2 = new TH1D("ph2", "", bins, min, max);
   TH1D* th2 = new TH1D("th2", "", bins, min, max);

   // 凡例
   TLegend* leg2 = new TLegend(0.1, 0.8, 0.3, 0.9);
  
   unsigned int m = tin1->GetEntries();
   for( unsigned int i = 0; i < m; i++ ) {

      tin1->GetEntry(i);

      if( pz > 0 ) {
      
         double phi = std::atan2(vy, vx);

         //if( i % 5000 == 0 ) std::cout << "phi = " << phi <<  std::endl;

         if( sqrt( x*x + y*y ) > 18.0 ) {

            if( k == 2 ) {	
               for(int j = 0; j < w; j++ ) eh2->Fill(phi);
            }
            else if( k == 3 ) {
               for( int j = 0; j < w; j++ ) ph2->Fill(phi);
            }
      
            for( int j = 0; j < w; j++ ) th2->Fill(phi);
         }
      }
   }

   // Statistics box, 0:非表示 1:表示
   th2->SetStats(0);
   eh2->SetStats(0);
   ph2->SetStats(0);

   // color設定
   th2->SetLineColor(1);
   eh2->SetLineColor(9);
   ph2->SetLineColor(46);

   // plot
   th2->Draw("same");
   eh2->Draw("same");
   ph2->Draw("same");

   // 凡例出力
   leg2->AddEntry(eh2, "Electron", "l");
   leg2->AddEntry(ph2, "Positron", "l");
   leg2->AddEntry(th2, "Total", "l");
   leg2->Draw();

   cvs->Print(pf);
   cvs->Close();
}

void plot_phi_after_analysis_with_hole2(string fn, const char* rootfiles) {

   int k;
   double x, y, z, w;
   double px, py, pz;
   double vx, vy;

   std::cout << "Processing plot_phi for " << fn << "..." << std::endl;

   std::string pdffile = fn + "/magnetic_field/" + fn + ".pdf";
   const char* pf = pdffile.c_str();
  
   // rootファイルを読み込み
   TFile* fin1 = new TFile(rootfiles, "read");
   TTree* tin1 = (TTree*)fin1->Get( "t" );

   tin1->SetBranchAddress("k", &k);
   tin1->SetBranchAddress("weight", &w);
   tin1->SetBranchAddress("x", &x);
   tin1->SetBranchAddress("y", &y);
   tin1->SetBranchAddress("z", &z);
   tin1->SetBranchAddress("px", &px);
   tin1->SetBranchAddress("py", &py);
   tin1->SetBranchAddress("pz", &pz);
   tin1->SetBranchAddress("vx", &vx);
   tin1->SetBranchAddress("vy", &vy);

   // Canvas生成
   TCanvas* cvs = new TCanvas("cvs", "cvs", 1000, 500);

   // y軸のタイトルのオフセット
   gStyle->SetTitleOffset(1.5, "y");

   cvs->Divide(2,1);
  
   cvs->cd(1);

   double bins = 30*M_PI;
   double min = -M_PI;
   double max = M_PI;
   int ent_min = 0;
   int ent_max = 120; 

   TH1F* frame1 = (TH1F*)cvs->DrawFrame(min, ent_min, max, ent_max, "#varphi with two hole (-4m); #varphi[rad]; Entries");
  
   TH1D* eh1 = new TH1D("eh1", "", bins, min, max);
   TH1D* ph1 = new TH1D("ph1", "", bins, min, max);
   TH1D* th1 = new TH1D("th1", "", bins, min, max);

   // 凡例
   TLegend* leg = new TLegend(0.1, 0.8, 0.3, 0.9);
  
   unsigned int n = tin1->GetEntries();
   for( unsigned int i = 0; i < n; i++ ) {

      tin1->GetEntry(i);

      if( pz < 0 ) {
      
         double phi = std::atan2(vy, vx);
         double r_large_beampipe = sqrt( std::pow(x, 2.0) + std::pow(y, 2.0) );
         double r_small_beampipe = sqrt( std::pow(x+56.0, 2.0 ) + std::pow(y, 2.0) );

         //if( i % 5000 == 0 ) std::cout << "phi = " << phi <<  std::endl;

         if( r_large_beampipe > 18.0 && r_small_beampipe > 10.0 ) {
            if( k == 2 ) {	
               for(int j = 0; j < w; j++ ) eh1->Fill(phi);
            }
            else if( k == 3 ) {
               for( int j = 0; j < w; j++ ) ph1->Fill(phi);
            }

            for( int j = 0; j < w; j++ ) th1->Fill(phi);
         }
      }
   }

   // Statistics box, 0:非表示 1:表示
   th1->SetStats(0);
   eh1->SetStats(0);
   ph1->SetStats(0);

   // color設定
   th1->SetLineColor(1);
   eh1->SetLineColor(9);
   ph1->SetLineColor(46);

   // plot
   th1->Draw("same");
   eh1->Draw("same");
   ph1->Draw("same");

   // 凡例出力
   leg->AddEntry(eh1, "Electron", "l");
   leg->AddEntry(ph1, "Positron", "l");
   leg->AddEntry(th1, "Total", "l");
   leg->Draw();

   cvs->cd(2);

   TH1F* frame2 = (TH1F*)cvs->DrawFrame(min, ent_min, max, ent_max, "#varphi with two hole (+4m); #varphi[rad]; Entries");

   TH1D* eh2 = new TH1D("eh2", "", bins, min, max);
   TH1D* ph2 = new TH1D("ph2", "", bins, min, max);
   TH1D* th2 = new TH1D("th2", "", bins, min, max);

   // 凡例
   TLegend* leg2 = new TLegend(0.1, 0.8, 0.3, 0.9);
  
   unsigned int m = tin1->GetEntries();
   for( unsigned int i = 0; i < m; i++ ) {

      tin1->GetEntry(i);

      if( pz > 0 ) {
      
         double phi = std::atan2(vy, vx);
         double r_large_beampipe = sqrt( std::pow(x, 2.0) + std::pow(y, 2.0) );
         double r_small_beampipe = sqrt( std::pow(x+56.0, 2.0 ) + std::pow(y, 2.0) );

         //if( i % 5000 == 0 ) std::cout << "phi = " << phi <<  std::endl;

         if( r_large_beampipe > 18.0 && r_small_beampipe > 10.0 ) {
            
            if( k == 2 ) {	
               for(int j = 0; j < w; j++ ) eh2->Fill(phi);
            }
            else if( k == 3 ) {
               for( int j = 0; j < w; j++ ) ph2->Fill(phi);
            }
      
            for( int j = 0; j < w; j++ ) th2->Fill(phi);
         }
      }
   }

   // Statistics box, 0:非表示 1:表示
   th2->SetStats(0);
   eh2->SetStats(0);
   ph2->SetStats(0);

   // color設定
   th2->SetLineColor(1);
   eh2->SetLineColor(9);
   ph2->SetLineColor(46);

   // plot
   th2->Draw("same");
   eh2->Draw("same");
   ph2->Draw("same");

   // 凡例出力
   leg2->AddEntry(eh2, "Electron", "l");
   leg2->AddEntry(ph2, "Positron", "l");
   leg2->AddEntry(th2, "Total", "l");
   leg2->Draw();

   cvs->Print(pf);
   cvs->Close();
}

void hist2d( string fn, const char* rootfiles ) {

   int k;
   double x, y, z, w, pz;

   int xmin = -100;
   int ymin = -100;
   int xmax = 100;
   int ymax = 100;

   std::cout << "Processing hist2d for " << fn << "..." << std::endl;

   std::string pdffile = fn + "/magnetic_field/" + fn +".pdf(";
   const char* pf = pdffile.c_str();
  
   // rootファイルを読み込み
   TFile* fin1 = new TFile(rootfiles, "read");
   TTree* tin1 = (TTree*)fin1->Get( "t" );

   tin1->SetBranchAddress("k", &k);
   tin1->SetBranchAddress("weight", &w);
   tin1->SetBranchAddress("x", &x);
   tin1->SetBranchAddress("y", &y);
   tin1->SetBranchAddress("z", &z);
   tin1->SetBranchAddress("pz", &pz);

   // Canvas生成
   TCanvas* cvs = new TCanvas("cvs", "cvs", 1000, 500);

   cvs->Divide(2,1);

   // y軸のタイトルのオフセット
   gStyle->SetTitleOffset(1.3, "y");
   gStyle->SetLabelOffset(-0.0, "z"); // 軸ラベルと軸との間の距離を設定する. -は軸に近づける
   gStyle->SetOptStat(0); // stat windowを非表示
   gStyle->SetPalette(kRainBow); // ヒストグラムで使用するカラーを設定する
  
   cvs->cd(1);

   // z軸のスケールをlogに変更
   gPad->SetLogz();

   TH2D *h1 = new TH2D("h1", "deposition of hit distribution (-z, e-); x[mm]; y[mm]", 100, xmin, xmax, 100, ymin, ymax);

   int n = tin1->GetEntries();
   for( int i = 0; i < n; i++ ) {

      tin1->GetEntry(i);

      double r = sqrt( x*x + y*y );

      if( pz < 0 && k == 2 && r > 18.0 ) {
         for( int a = 0; a < w; a++ ) h1->Fill(x, y); // weightの数だけbinに詰める
      }
   }
   h1->GetZaxis()->SetRangeUser(10, 1000);
   h1->Draw("COLZ");

   cvs->cd(2);

   // z軸のスケールをlogに変更
   gPad->SetLogz();

   TH2D *h2 = new TH2D("h2", "deposition of hit distribution (+z, e-); x[mm]; y[mm]", 100, xmin, xmax, 100, ymin, ymax);

   int m = tin1->GetEntries();
   for( int i = 0; i < m; i++ ) {

      tin1->GetEntry(i);

      double r = sqrt( x*x + y*y );

      if( pz > 0  && k == 2 && r > 18.0 ) {
         for( int a = 0; a < w; a++ ) h2->Fill(x, y);
      }
   }
   h2->GetZaxis()->SetRangeUser(10, 1000);
   h2->Draw("COLZ");

   cvs->Print(pf);

   cvs->cd(1);

   // z軸のスケールをlogに変更
   gPad->SetLogz();

   TH2D *h3 = new TH2D("h3", "deposition of hit distribution (-z, e+); x[mm]; y[mm]", 100, xmin, xmax, 100, ymin, ymax);

   int o = tin1->GetEntries();
   for( int i = 0; i < o; i++ ) {

      tin1->GetEntry(i);

      double r = sqrt( x*x + y*y );

      if( pz < 0 && k == 3 && r > 18.0 ) {
         for( int a = 0; a < w; a++ ) h3->Fill(x, y);
      }
   }
   h3->GetZaxis()->SetRangeUser(10, 1000);
   h3->Draw("COLZ");

   cvs->cd(2);

   // z軸のスケールをlogに変更
   gPad->SetLogz();

   TH2D *h4 = new TH2D("h4", "deposition of hit distribution (+z, e+); x[mm]; y[mm]", 100, xmin, xmax, 100, ymin, ymax);

   int q = tin1->GetEntries();
   for( int i = 0; i < q; i++ ) {

      tin1->GetEntry(i);

      double r = sqrt( x*x + y*y );

      if( pz > 0 && k == 3 && r > 18.0 ) {
         for( int a = 0; a < w; a++ ) h4->Fill(x, y);
      }
   }
   h4->GetZaxis()->SetRangeUser(10, 1000);
   h4->Draw("COLZ");

   cvs->Print(pf);
  
   cvs->cd(1);

   // z軸のスケールをlogに変更
   gPad->SetLogz();

   TH2D *h5 = new TH2D("h5", "deposition of hit distribution (-z, total); x[mm]; y[mm]", 100, xmin, xmax, 100, ymin, ymax);

   int p = tin1->GetEntries();
   for( int i = 0; i < p; i++ ) {

      tin1->GetEntry(i);

      double r = sqrt( x*x + y*y );

      if( pz < 0 && r > 18.0 ) {
         for( int a = 0; a < w; a++ ) h5->Fill(x, y);
      }
   }
   h5->GetZaxis()->SetRangeUser(10, 1000);
   h5->Draw("COLZ");

   cvs->cd(2);

   // z軸のスケールをlogに変更
   gPad->SetLogz();

   TH2D *h6 = new TH2D("h6", "deposition of hit distribution (+z, total); x[mm]; y[mm]", 100, xmin, xmax, 100, ymin, ymax);


   int l = tin1->GetEntries();
   for( int i = 0; i < l; i++ ) {

      tin1->GetEntry(i);

      double r = sqrt( x*x + y*y );

      if( pz > 0 && r > 18.0 ) {
         for( int a = 0; a < w; a++ ) h6->Fill(x, y);
      }
   }
   h6->GetZaxis()->SetRangeUser(10, 1000);
   h6->Draw("COLZ");

   cvs->Print(pf);
   cvs->Close();
}

void hist2d_with_hole2( string fn, const char* rootfiles ) {

   int k;
   double x, y, z, w, pz;

   int xmin = -100;
   int ymin = -100;
   int xmax = 100;
   int ymax = 100;

   std::cout << "Processing hist2d for " << fn << "..." << std::endl;

   std::string pdffile = fn + "/magnetic_field/" + fn +".pdf";
   std::string pdffile_end = fn + "/magnetic_field/" + fn + ".pdf)";
   const char* pf = pdffile.c_str();
   const char* pfe = pdffile_end.c_str();
  
   // rootファイルを読み込み
   TFile* fin1 = new TFile(rootfiles, "read");
   TTree* tin1 = (TTree*)fin1->Get( "t" );

   tin1->SetBranchAddress("k", &k);
   tin1->SetBranchAddress("weight", &w);
   tin1->SetBranchAddress("x", &x);
   tin1->SetBranchAddress("y", &y);
   tin1->SetBranchAddress("z", &z);
   tin1->SetBranchAddress("pz", &pz);

   // Canvas生成
   TCanvas* cvs = new TCanvas("cvs", "cvs", 1000, 500);

   cvs->Divide(2,1);

   // y軸のタイトルのオフセット
   gStyle->SetTitleOffset(1.3, "y");
   gStyle->SetLabelOffset(-0.0, "z"); // 軸ラベルと軸との間の距離を設定する. -は軸に近づける
   gStyle->SetOptStat(0); // stat windowを非表示
   gStyle->SetPalette(kRainBow); // ヒストグラムで使用するカラーを設定する
  
   cvs->cd(1);

   // z軸のスケールをlogに変更
   gPad->SetLogz();

   TH2D *h1 = new TH2D("h1", "deposition of hit distribution (-z, e-); x[mm]; y[mm]", 100, xmin, xmax, 100, ymin, ymax);

   int n = tin1->GetEntries();
   for( int i = 0; i < n; i++ ) {

      tin1->GetEntry(i);

      double r_large_beampipe = sqrt( x*x + y*y );
      double r_small_beampipe = sqrt( std::pow( x + 56.0, 2.0) + std::pow(y, 2.0) );

      if( pz < 0 && k == 2 && r_large_beampipe > 18.0 && r_small_beampipe > 10.0 ) {
         for( int a = 0; a < w; a++ ) h1->Fill(x, y); // weightの数だけbinに詰める
      }
   }
   h1->GetZaxis()->SetRangeUser(10, 1000);
   h1->Draw("COLZ");

   cvs->cd(2);

   // z軸のスケールをlogに変更
   gPad->SetLogz();

   TH2D *h2 = new TH2D("h2", "deposition of hit distribution (+z, e-); x[mm]; y[mm]", 100, xmin, xmax, 100, ymin, ymax);

   int m = tin1->GetEntries();
   for( int i = 0; i < m; i++ ) {

      tin1->GetEntry(i);

      double r_large_beampipe = sqrt( x*x + y*y );
      double r_small_beampipe = sqrt( std::pow(x+56.0, 2.0) + std::pow(y, 2.0));

      if( pz > 0  && k == 2 && r_large_beampipe > 18.0 && r_small_beampipe > 10.0 ) {
         for( int a = 0; a < w; a++ ) h2->Fill(x, y);
      }
   }
   h2->GetZaxis()->SetRangeUser(10, 1000);
   h2->Draw("COLZ");

   cvs->Print(pf);

   cvs->cd(1);

   // z軸のスケールをlogに変更
   gPad->SetLogz();

   TH2D *h3 = new TH2D("h3", "deposition of hit distribution (-z, e+); x[mm]; y[mm]", 100, xmin, xmax, 100, ymin, ymax);

   int o = tin1->GetEntries();
   for( int i = 0; i < o; i++ ) {

      tin1->GetEntry(i);

      double r_large_beampipe = sqrt( x*x + y*y );
      double r_small_beampipe = sqrt( std::pow(x+56.0, 2.0) + std::pow(y,2.0) );

      if( pz < 0 && k == 3 && r_large_beampipe > 18.0 && r_small_beampipe > 10.0 ) {
         for( int a = 0; a < w; a++ ) h3->Fill(x, y);
      }
   }
   h3->GetZaxis()->SetRangeUser(10, 1000);
   h3->Draw("COLZ");

   cvs->cd(2);

   // z軸のスケールをlogに変更
   gPad->SetLogz();

   TH2D *h4 = new TH2D("h4", "deposition of hit distribution (+z, e+); x[mm]; y[mm]", 100, xmin, xmax, 100, ymin, ymax);

   int q = tin1->GetEntries();
   for( int i = 0; i < q; i++ ) {

      tin1->GetEntry(i);

      double r_large_beampipe = sqrt( x*x + y*y );
      double r_small_beampipe = sqrt( std::pow(x+56.0, 2.0) + std::pow(y, 2.0) );

      if( pz > 0 && k == 3 && r_large_beampipe > 18.0 && r_small_beampipe > 10.0 ) {
         for( int a = 0; a < w; a++ ) h4->Fill(x, y);
      }
   }
   h4->GetZaxis()->SetRangeUser(10, 1000);
   h4->Draw("COLZ");

   cvs->Print(pf);
  
   cvs->cd(1);

   // z軸のスケールをlogに変更
   gPad->SetLogz();

   TH2D *h5 = new TH2D("h5", "deposition of hit distribution (-z, total); x[mm]; y[mm]", 100, xmin, xmax, 100, ymin, ymax);

   int p = tin1->GetEntries();
   for( int i = 0; i < p; i++ ) {

      tin1->GetEntry(i);

      double r_large_beampipe = sqrt( x*x + y*y );
      double r_small_beampipe = sqrt( std::pow(x+56.0, 2.0) + std::pow(y, 2.0 ));

      if( pz < 0 && r_large_beampipe > 18.0 && r_small_beampipe > 10.0 ) {
         for( int a = 0; a < w; a++ ) h5->Fill(x, y);
      }
   }
   h5->GetZaxis()->SetRangeUser(10, 1000);
   h5->Draw("COLZ");

   cvs->cd(2);

   // z軸のスケールをlogに変更
   gPad->SetLogz();

   TH2D *h6 = new TH2D("h6", "deposition of hit distribution (+z, total); x[mm]; y[mm]", 100, xmin, xmax, 100, ymin, ymax);


   int l = tin1->GetEntries();
   for( int i = 0; i < l; i++ ) {

      tin1->GetEntry(i);

      double r_large_beampipe = sqrt( x*x + y*y );
      double r_small_beampipe = sqrt( std::pow(x+56.0, 2.0) + std::pow(y, 2.0) );

      if( pz > 0 && r_large_beampipe > 18.0 && r_small_beampipe > 10.0 ) {
         for( int a = 0; a < w; a++ ) h6->Fill(x, y);
      }
   }
   h6->GetZaxis()->SetRangeUser(10, 1000);
   h6->Draw("COLZ");

   cvs->Print(pfe);
   cvs->Close();
}

void plotGeneralOutput( string inputdir ) {

   string inputroot = inputdir + "/magnetic_field/inPairOnMF.root";
   const char* rt = inputroot.c_str();
      
   position2d( inputdir, rt ); // 必ず実行pdf
   plot_phi( inputdir, rt ); // 選択可
   plot_phi_after_analysis( inputdir, rt ); // 選択可
   plot_phi_after_analysis_with_hole( inputdir, rt ); // 選択可
   plot_phi_after_analysis_with_hole2( inputdir, rt ); // 選択可
   hist2d( inputdir, rt ); // 選択可
   hist2d_with_hole2( inputdir, rt ); // 必ず実行 pdfのため

}

