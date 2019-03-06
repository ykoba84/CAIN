#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <sys/stat.h>

using namespace std;

void convertDatToRootfiles( string inputDirectory  ) {

   /**********User指定ここから**********/
   
   // ディレクトリを指定する
   //string inputDirectory = "Run_SigxFactor1.0_SigyFactor5.0_DeltayFactor0.0";
   
   // in_*のファイル読み込み設定
   int GRID = 1000; // グリッド幅を指定
   int MESH = 1; // メッシュ数を指定
   
   /**********User指定ここまで**********/

   const char* dp = inputDirectory.c_str();
   struct stat statBuf;
   
   if( stat( dp, &statBuf ) != 0 ) {
      cout << "not exist the directory." << endl;
      exit(1);
   }

   cout << "workDirectory : " << inputDirectory << endl;
      
   // 出力ファイル
   string outputFile = inputDirectory + "/" + "inPair.root";
   const char* ofp = outputFile.c_str();

   // 出力rootファイルを読み込む
   TFile* f = new TFile(ofp, "RECREATE");

   for( int i = 1; i <= MESH; i++ ) {

      int k, gen, n=GRID;
      double weight, t, x, y, s, energy, px, py, ps, sx, sy, ss;
      string str, name;

      // in_ファイルを読み込む
      string inFile = "in_" + to_string( i * n );
      string inDirectoryFile = inputDirectory + "/" + inFile;
      cout << "FileName = " << inFile << endl;
      const char* ip = inFile.c_str();

      TTree* tt = new TTree(ip, "incoherent pair");

      inDirectoryFile += ".dat";
      const char* infilename = inDirectoryFile.c_str();

      tt->Branch("k", &k, "k/I"); // Particle species. 1,2,3 for photon, electron, positron
      tt->Branch("gen", &gen, "gen/I"); // Particle generation
      tt->Branch("weight", &weight, "weight/D");  // Macro-Particle weight
      tt->Branch("t", &t, "t/D"); // particle time-space coordinate ( m )
      tt->Branch("x", &x, "x/D");
      tt->Branch("y", &y, "y/D");
      tt->Branch("s", &s, "s/D");
      tt->Branch("energy", &energy, "energy/D");  // energy-momentum ( eV, eV/c )
      tt->Branch("px", &px, "px/D");
      tt->Branch("py", &py, "py/D");
      tt->Branch("ps", &ps, "ps/D");
      tt->Branch("sx", &sx, "sx/D");  // Electron/positron spin
      tt->Branch("sy", &sy, "sy/D");
      tt->Branch("ss", &ss, "ss/D");

      // in_ファイルを読み込む
      ifstream ifs( inDirectoryFile );

      // 1行目は変数名なので除外する(読み込むだけ)
      getline(ifs, str);

      // 値を読み込む
      while( ifs >> k >> gen >> name >> weight >> t >> x >> y >> s >> energy >> px >> py >> ps >> sx >> sy >> ss ) {

         // x-y-s座標でright-handedからleft-handedに変更する
         // x *= -1.0;
         // px *= -1.0;
         // sx *= -1.0;
         
         // m->um
         x /= 1.0E-6;
         y /= 1.0E-6;
         s /= 1.0E-6;

         //cerr << "x = " << x << "y = " << y << endl;
         tt->Fill();

      }
      // ファイル書き込み
      tt->Write();
      // ファイル削除
      ifs.close();
      int err = remove( infilename );
      if( err != 0 )
         std::cout << "error" << std::endl;
      
   }

   cout << "Completed!" << endl;

   f->Close();
}

