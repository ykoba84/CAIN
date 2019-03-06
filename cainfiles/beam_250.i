!     nominal parameter set of the ILC
SET   incpair=1;
SET $ttl='B1', travelingfocus=1;
SET $in_pairfile='incoherent_pair.dat';
SET $co_pairfile='coherent_pair.dat';

SET Rand=50000;
ALLOCATE MP=2000000, MVPH=1000000, MLUMMESH=256, ;

SET photon=1, electron=2, positron=3,
mm=1D-3, micron=1D-6, nm=1D-9, mu0=4*Pi*1D-7,
psec=1e-12*Cvel;

FLAG ON SPIN;

SET an=2.0E10, nb=1312, emx=1.0E-5, emy=3.5E-8;
! emy=3.5E-8
      
!     ILC 250eV setting;
! by=0.41*nm
SET sz=0.3*mm, bx=13.0*mm, by=0.41*mm,
    e_spread=0.190, p_spread=0.152;

IF travelingfocus;
   SET $ttl=$ttl+'wTF';
ELSE;
   SET $ttl=$ttl+'nTF';
ENDIF;

HEADER   $ttl;
SET  
   ee=125E9,  gamma=ee/Emass, nbunch=nb, reprate=5,
   sigz=sz, betax=bx, betay=by,
   emitx=emx/gamma,  emity=emy/gamma,
   nx=4.5, ny=4.5, nt=4.5, ne=4.5,
   dalphadty=0, s0=0, 
   sigx=Sqrt(emitx*betax), sigy=Sqrt(emity*betay),
   sigxp=sigx/betax,  sigyp=sigy/betay,
   slopex=0.000, slopey=0.000;
      
IF travelingfocus > 0;
   SET    s0=Sqrt(3)/2*sigz, dalphadty=-0.5/betay;
ENDIF;

SET MsgLevel=1;

BEAM
   RIGHT, KIND=electron, NP=100000, AN=an, E0=ee,
   EUNIFORM, TXYS=(-0.8*mm, 0, 0, -s0), SPIN=(0,0,-1),
   BETA=(betax,betay), EMIT=(emitx,emity), SIGT=sigz,GCUT=(nx,ny),
   DALPHADT=(0, dalphadty), SIGE=e_spread,
   GCUTT=nt, GCUTE=ne, SLOPE=(slopex,slopey), CRAB=(slopex,slopey);


BEAM
   LEFT,  KIND=positron, NP=100000, AN=an, E0=ee,
   EUNIFORM, TXYS=(-0.8*mm, 0, 0, s0), SPIN=(0,0,1),
   BETA=(betax,betay), EMIT=(emitx,emity), SIGT=sigz,GCUT=(nx,ny),
   DALPHADT=(0, dalphadty), SIGE=p_spread,
   GCUTT=nt, GCUTE=ne, SLOPE=(slopex,slopey), CRAB=(slopex,slopey);

!     Longitudinal mesh size for luminosity and beam-field calculation;

SET Smesh=0.01*sigz;

!     Define luminosities to be calculated;
LUMINOSITY  KIND=(2,3), W=(0,2*1.000000001*ee,1000), HELICITY, 
   WX=12*sigx, WY=12*sigy,  FREP=nbunch*reprate ;

!     standard WX=8*sigx, WY=8*sigy,  FREP=nbunch*reprate ; temporarily changed for TF;

LUMINOSITY  KIND=(2,1), W=(0,2*1.01*ee,101);
LUMINOSITY  KIND=(3,1), W=(0,2*1.01*ee,101);
LUMINOSITY  KIND=(1,1), W=(0,2*1.01*ee,101);
LUMINOSITY  KIND=(1,2), W=(0,2*1.01*ee,101);
LUMINOSITY  KIND=(1,3), W=(0,2*1.01*ee,101);

!     Define parameters for beam-beam field calculation;
BBFIELD  NX=64, NY=128, WX=24*sigx, R=sigx/sigy/2;

!     standard NX=32, NY=128, WX=12*sigx, R=sigx/sigy/2; temporarily changed for TF;

!     Turn on beamstrahlung;
CFQED    BEAMSTRAHLUNG, POLARIZATION, PMAX=0.5;

!     Turn on coherent pair (ESLOPE has to be set!!!);
CFQED    PAIRCREATION, POLARIZATION, PMAX=0.5;
!     Define External field
EXTERNALFIELD B=(0.0, 0.0, 3.5);
      
!     Turn on incoherent pair
IF incpair > 0;
   SET enhpp=1;
   PPINT VIRTUALPHOTON, FIELDSUPPRESSION, EMIN=3.0E06;
   PPINT BW, ENHANCE=enhpp;
   PPINT BH, ENHANCE=enhpp;
   PPINT LL, ENHANCE=enhpp;
ENDIF;

SET MsgLevel=0;

FLAG OFF ECHO;

SET it=0, nstep=1000;

!     Start tracking
!     Time=(-nt*sigz,nt*sigz,nstep);
PUSH  Time=(-nt*sigz,nt*sigz,nstep);

      IF Mod(it,1000)=0;
         PRINT $ttl, it, 
         FORMAT=(' +++ ',A,' +++',F5.0,'-th time step +++');
         PRINT STAT, SHORT;
         IF incpair > 0;
            PRINT STAT, INCP, SHORT;
            WRITE BEAM, INCP, FILE='in_' + $ItoA(it) + '.dat';
         ENDIF;
      ENDIF;
      SET it=it+1;

ENDPUSH;

!WRITE BEAM, INCP, FILE='incoherent_pair.dat';

!     PLOT LUMINOSITY, KIND=(2,3), VLOG;
FLAG ON ECHO;      

!     Pull all particles back to the plane s=0
!     DRIFT EXTERNAL;
      
PRINT STAT;
PRINT STAT, INCP;
      
PRINT Lum(2,3),FORMAT=('luminosity_ee=',1pd15.8);
SET n1=LumWbin(2,3,0);
SET lumpeak=LumW(2,3,n1)+LumW(2,3,n1-1);
PRINT lumpeak/Lum(2,3)*100,FORMAT=('luminosity at 1% =',0pF10.3,' %');

PRINT NParticle(1,1)/an, NParticle(2,1)/an, (NParticle(1,1)+NParticle(2,1))/(2*an),
FORMAT=(' Average number of photons =',0p3F10.4);
SET   deltaB1=(ee-AvrEn(1,2))/ee, deltaB2=(ee-AvrEn(2,3))/ee;
PRINT deltaB1*100,deltaB2*100,(deltaB1+deltaB2)/2*100,
FORMAT=(' Average energy loss =',0p3F10.3,' %');

PRINT Lum(1,1),FORMAT=('luminosity_gam gam=',1pd15.8);
PRINT Lum(2,1),FORMAT=('luminosity_gam e1=',1pd15.8);
PRINT Lum(3,1),FORMAT=('luminosity_gam e2=',1pd15.8);
PRINT Lum(1,2),FORMAT=('luminosity_gam e1=',1pd15.8);
PRINT Lum(1,3),FORMAT=('luminosity_gam e2=',1pd15.8);

CLEAR BEAM;
CLEAR LUMINOSITY;
CLEAR BBFIELD;

STOP;
