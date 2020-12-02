#include "DP_simu/CALConstruct.hh"
#include "G4Box.hh"
#include "G4PVPlacement.hh"
#include "G4SubtractionSolid.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include <iterator>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CALConstruct::CALConstruct(G4String CALName, 
                           G4LogicalVolume* MotherVolume, 
                           G4int CopyNo, 
                           G4bool Type, 
                           G4bool Wrap, 
                           G4bool Optical,  /* if simulate optical photon */
                           G4bool CheckOverlap)
{
    fCALName = CALName;
    fMotherVolume = MotherVolume;
    fCopyNo = CopyNo;
    fType = Type;
    fWrap = Wrap;
    fOptical = Optical;
    fCheckOverlap = CheckOverlap;

    fSizeX = 0.; 
    fSizeY = 0.; 
    fSizeZ = 0.; 
    fPosX = 0.; 
    fPosY = 0.; 
    fPosZ = 0.; 
    fWrapSizeX = 0.; 
    fWrapSizeY = 0.; 
    fWrapSizeZ = 0.; 

    fCALSD = nullptr;
    fCALWrapSD = nullptr;
    fVis = nullptr;
    fWrapVis = nullptr;
    fCALMaterial = nullptr;
    fWrapMaterial = nullptr;

    ifAbsorber = false;
    fRecordLV = true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CALConstruct::CALConstruct(const CALConstruct& in)
{
    fCALName        = in.fCALName        ;
    fMotherVolume   = in.fMotherVolume   ;
    fCopyNo         = in.fCopyNo         ;
    fType           = in.fType           ;
    fWrap           = in.fWrap           ;
    fOptical        = in.fOptical        ;
    fCheckOverlap   = in.fCheckOverlap   ;
    fSizeX          = in.fSizeX          ;
    fSizeY          = in.fSizeY          ;
    fSizeZ          = in.fSizeZ          ;
    fPosX           = in.fPosX           ;
    fPosY           = in.fPosY           ;
    fPosZ           = in.fPosZ           ;
    fWrapSizeX      = in.fWrapSizeX      ;
    fWrapSizeY      = in.fWrapSizeY      ;
    fWrapSizeZ      = in.fWrapSizeZ      ;
    fCALMaterial    = in.fCALMaterial    ;
    fWrapMaterial   = in.fWrapMaterial   ;
    fCALSD          = in.fCALSD          ;
    fCALWrapSD      = in.fCALWrapSD      ;
    fVis            = in.fVis            ;
    fWrapVis        = in.fWrapVis        ;
    fCaloLVVector   = in.fCaloLVVector   ;
    fWrapLVVector   = in.fWrapLVVector   ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
CALConstruct::~CALConstruct()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreeVector CALConstruct::Construct()
{
    auto TotalSize = G4ThreeVector(0,0,0);
    // check consistency
    if ( !fSizeX || !fSizeY || !fSizeZ ) 
    {
        G4cout<<fCALName<<" Construction Error: at least size of one dimension is zero."<<G4endl; 
        return TotalSize;
    }

    // Placement of Calorimeter
    auto pos = G4ThreeVector(fPosX, fPosY, fPosZ);

    auto fName = ifAbsorber ? fCALName+"Abs" : fCALName;
    auto box = new G4Box(fName+"_Box" , fSizeX, fSizeY, fSizeZ );
    auto boxLV = new G4LogicalVolume(box, fCALMaterial, fName+"_LV", 0,0,0);
    auto boxPV = new G4PVPlacement(nullptr, pos, boxLV, fName+"_PV", fMotherVolume, false, fCopyNo, fCheckOverlap);

    if ( fRecordLV ) fCaloLVVector.push_back(boxLV);

    if ( fVis ) 
    {
        fVis->SetVisibility(true); 
        boxLV->SetVisAttributes(fVis);
    }
    else boxLV->SetVisAttributes(G4VisAttributes::GetInvisible());
    if ( fCALSD ) boxLV->SetSensitiveDetector( fCALSD );

    TotalSize = G4ThreeVector(fSizeX, fSizeY, fSizeZ);
    
    // Placement of Wrap
    if ( fWrap )
    {
        auto boxL = new G4Box(fCALName+"_BoxL" , fSizeX+fWrapSizeX, 
                                                 fSizeY+fWrapSizeY, 
                                                 fSizeZ+fWrapSizeZ);
        auto WrapBox = new G4SubtractionSolid(fCALName+"_BoxW", boxL, box);
        auto WrapLV = new G4LogicalVolume(WrapBox, fWrapMaterial, fCALName+"_LVW", 0,0,0);
        auto WrapPV = new G4PVPlacement(0, pos, WrapLV, fCALName+"_PVW", fMotherVolume, false, fCopyNo, fCheckOverlap);
        
        if ( fWrapVis ) 
        {
            fWrapVis->SetVisibility(true); 
            WrapLV->SetVisAttributes(fWrapVis);
        }
        else WrapLV->SetVisAttributes(G4VisAttributes::GetInvisible());

        if ( fCALWrapSD ) WrapLV->SetSensitiveDetector( fCALWrapSD );
        
        TotalSize = G4ThreeVector(fSizeX+fWrapSizeX, fSizeY+fWrapSizeY, fSizeZ+fWrapSizeZ);

        if ( fOptical ) 
        {
            /* Set Optical Porperties for boundary surface */
            auto* WrapSurface = new G4OpticalSurface( fCALName+"WrapSurface" );
            //WrapSurface->SetType(dielectric_dielectric);
            //WrapSurface->SetFinish(polished);
            //WrapSurface->SetModel(glisur);

            WrapSurface->SetType(dielectric_LUT);
            WrapSurface->SetFinish(polishedtyvekair);
            WrapSurface->SetModel(LUT);
            //new G4LogicalSkinSurface( fCALName+"WrapSurface", WrapLV, WrapSurface );
            new G4LogicalBorderSurface( fCALName+"WrapSurface", boxPV, WrapPV, WrapSurface);

            // test
            const G4int num = 2;
            G4double ephoton[num] = {1*eV, 7*eV};
            G4double reflectivity[num] = {1.0, 1.0};
            G4double efficiency[num]   = {1.0, 1.0};
            
            auto *myST2 = new G4MaterialPropertiesTable();
            myST2->AddProperty("REFLECTIVITY", ephoton, reflectivity, num);
            myST2->AddProperty("EFFICIENCY",   ephoton, efficiency,   num);

            //WrapSurface->SetMaterialPropertiesTable(myST2);

            /* Placement of APD */
            auto abox = new G4Box(fCALName+"_APDWorld_Box" , wSizeX, wSizeY, wSizeZ );
            auto aboxLV = new G4LogicalVolume(abox, G4Material::GetMaterial("vacuum"), fCALName+"_APDWorld_LV", 0,0,0);
            new G4PVPlacement( 0, G4ThreeVector(wPosX, wPosY, wPosZ), aboxLV, fCALName+"_APDWorld_PV", boxLV, false, fCopyNo, fCheckOverlap);

            auto* wVis = new G4VisAttributes(G4Colour(0.5,0.5,.0));
            wVis->SetVisibility(true);
            aboxLV->SetVisAttributes(wVis);

        }

    }

    return TotalSize;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreeVector CALConstruct::MatrixPlacement(G4int xNo, G4int yNo, G4int zNo, const G4ThreeVector& CentrePos)
{
    auto TotalSize = G4ThreeVector(0,0,0);
    // check consistency
    if ( !xNo || !yNo || !zNo ) 
    {
        G4cout<<fCALName<<" Construction Error: at least one of the matrix element is zero."<<G4endl; 
        return TotalSize;
    }

    TotalSize = G4ThreeVector( xNo*(fSizeX + fWrap*fWrapSizeX ),
                               yNo*(fSizeY + fWrap*fWrapSizeY ),
                               zNo*(fSizeZ + fWrap*fWrapSizeZ ) );    

    for (int k = 0; k < zNo; k++) 
    {
      for (int j = 0; j < yNo; j++)
      {
        for (int i = 0; i < xNo; i++)
        {
          fPosX = -1.*TotalSize.x() + (2*i+1)*(fSizeX + fWrap*fWrapSizeX ) + CentrePos.x(); 
          fPosY = -1.*TotalSize.y() + (2*j+1)*(fSizeY + fWrap*fWrapSizeY ) + CentrePos.y(); 
          fPosZ = -1.*TotalSize.z() + (2*k+1)*(fSizeZ + fWrap*fWrapSizeZ ) + CentrePos.z();

          wSizeX = fSizeX;
          wSizeY = fSizeY;
          wSizeZ = aSizeZ + gSizeZ;
          wPosX  = 0.;
          wPosY  = 0.;
          wPosZ  = fSizeZ - 0.5*wSizeZ;

          Construct();  
          
          fCopyNo++;
        }
      }
    }

    return TotalSize;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CALConstruct:: MatrixPlacementXYRemoved(G4int xNo, G4int yNo, G4int zNo, G4ThreeVector CentrePos, G4int NoRemoved, G4int type) 
{
    /* Type: 
     * 1) Left Bottom Corner
     * 4) Right Top Corner
     * 2) Right Bottom Corner
     * 3) Left Top Corner
     */
    auto idx = xNo;
    auto idy = yNo;
    auto idz = zNo;

    auto iSizeX = fSizeX;
    auto iSizeY = fSizeY;

    auto TotalSize = G4ThreeVector( xNo*(fSizeX + fWrap*fWrapSizeX ),
                                    yNo*(fSizeY + fWrap*fWrapSizeY ),
                                    zNo*(fSizeZ + fWrap*fWrapSizeZ ) );    

    for (int k = 0; k < idz; k++) 
    {
      // along x direction
      if ( k%2 == 0 ) { idx = xNo; idy = yNo; fSizeX = iSizeX; fSizeY  = iSizeY; }
      // along y direction
      if ( k%2 == 1 ) { idx = yNo; idy = xNo; fSizeX = iSizeY; fSizeY  = iSizeX; }
      for (int j = 0; j < idy; j++)
      {
        for (int i = 0; i < idx; i++)
        {
          auto tmpX = ((k%2 == 0) ? iSizeX : iSizeY);
          auto tmpY = ((k%2 == 1) ? iSizeX : iSizeY);


          fPosX = -1.*TotalSize.x() + (2*i+1)*(tmpX + fWrap*fWrapSizeX ) + CentrePos.x(); 
          fPosY = -1.*TotalSize.y() + (2*j+1)*(tmpY + fWrap*fWrapSizeY ) + CentrePos.y(); 
          fPosZ = -1.*TotalSize.z() + (2*k+1)*(fSizeZ + fWrap*fWrapSizeZ ) + CentrePos.z();

          double w1 = 0;
          double w2 = 0;
          double w3 = 0;
          double w4 = 0;
          double w5 = 0;
          double w6 = 0;

          if( type == 1 ) { w1 = 1; w2 = -1; w3 = NoRemoved; w4 = -1; w5 = 1; w6 = -(idx-NoRemoved-1); } 
          if( type == 4 ) { w1 =-1; w2 =  1; w3 = -(idy-NoRemoved-1); w4 = 1; w5 = -1; w6 = NoRemoved; } 
          if( type == 2 ) { w1 = 1; w2 =  1; w3 = NoRemoved; w4 =  1; w5 = 1; w6 =          NoRemoved; } 
          if( type == 3 ) { w1 =-1; w2 = -1; w3 = -(idy-NoRemoved-1); w4 =-1; w5 = -1; w6 = -(idx-NoRemoved-1); } 
              
          if ( k%2 == 0 ) 
          {
            fSizeX = iSizeX -    (( (w1*j) < w3 ) ? NoRemoved*(iSizeY+fWrap*fWrapSizeY): 0.);
            fPosX  = fPosX  + w2*(( (w1*j) < w3 ) ? NoRemoved*(iSizeY+fWrap*fWrapSizeY): 0.);
          }
          else
          {
            fSizeY = iSizeX -    (( (w4*i) < w6 ) ? NoRemoved*(iSizeY+fWrap*fWrapSizeX): 0.);
            fPosY  = fPosY  + w5*(( (w4*i) < w6 ) ? NoRemoved*(iSizeY+fWrap*fWrapSizeX): 0.);
          }

          /* APD */
          double f1 = 0,f2 = 0;
          if( type == 1 ) { f1 = -1; f2 =  1; }
          if( type == 2 ) { f1 =  1; f2 =  1; }
          if( type == 3 ) { f1 = -1; f2 = -1; }
          if( type == 4 ) { f1 =  1; f2 = -1; }

          wSizeZ = fSizeZ;
          wPosZ = 0.;
          if ( k%2 == 0 ) {
            wSizeX = aSizeZ + gSizeZ;
            wSizeY = fSizeY;
            wPosY  = 0.;

            wPosX  = f1*fSizeX - f1*0.5*wSizeX;
          } else if ( k%2 == 1 ) {
            wSizeX = fSizeX;
            wSizeY = aSizeZ + gSizeZ;
            wPosX  = 0.;

            wPosY  = f2*fSizeY - f2*0.5*wSizeY;
          }


          Construct();  
          fCopyNo++;
        }
      }
    }
    
    return;
}

void CALConstruct::MatrixPlacementXYwithAbsorber(G4int xNo, G4int yNo, G4int zNo, G4ThreeVector CentrePos, G4double AbsThickness, G4Material* AbsMat) {

    auto Mat = fCALMaterial;
    auto ifwrap = fWrap;
    auto idx = xNo;
    auto idy = yNo;
    auto idz = zNo + zNo/2;
    auto iVis = fVis;

    auto iSizeX = fSizeX;
    auto iSizeY = fSizeY;
    auto iSizeZ = fSizeZ;

    auto TotalSize = G4ThreeVector( xNo*(fSizeX + fWrap*fWrapSizeX ),
                                    yNo*(fSizeY + fWrap*fWrapSizeY ),
                                    zNo*(fSizeZ + fWrap*fWrapSizeZ ) + zNo/2.*AbsThickness/2. );    

    auto Abs_No = 0;
    for (int k = 0; k < idz; k++) 
    {
      // along x direction
      if ( k%3 == 0 ) { idx = xNo; idy = yNo; fSizeX = iSizeX; fSizeY  = iSizeY; }
      // along y direction
      if ( k%3 == 1 ) { idx = yNo; idy = xNo; fSizeX = iSizeY; fSizeY  = iSizeX; }
      // place absorber
      if ( k%3 == 2 ) { idx = 1; idy = 1; fSizeX = xNo*(iSizeX + fWrap*fWrapSizeX ); fSizeY  = yNo*(iSizeY + fWrap*fWrapSizeY ); }

      for (int j = 0; j < idy; j++)
      {
        for (int i = 0; i < idx; i++)
        {
          auto tmpX = ((k%3 == 0) ? iSizeX : iSizeY);
          auto tmpY = ((k%3 == 1) ? iSizeX : iSizeY);

          fPosX = -1.*TotalSize.x() + (2*i+1)*(tmpX + fWrap*fWrapSizeX ) + CentrePos.x(); 
          fPosY = -1.*TotalSize.y() + (2*j+1)*(tmpY + fWrap*fWrapSizeY ) + CentrePos.y(); 
          fPosZ = -1.*TotalSize.z() + (2*(k-Abs_No)+1)*(fSizeZ + fWrap*fWrapSizeZ ) + Abs_No*AbsThickness + CentrePos.z();

          if ( k%3 == 2 ) {
              Abs_No++;
              ifAbsorber = true;

              fPosX = CentrePos.x();
              fPosY = CentrePos.y();
              fPosZ = -1.*TotalSize.z() + (2*(k-Abs_No)+2)*(fSizeZ + fWrap*fWrapSizeZ ) + (Abs_No-0.5)*AbsThickness + CentrePos.z();

              fSizeZ = AbsThickness/2.;
              fWrap = false;
              fCALMaterial = AbsMat;
              fVis = new G4VisAttributes(G4Colour(0.5,0.23,0.89));
              fRecordLV = false;
           } else {
              ifAbsorber = false;

              /* APD */
              double f1 = 1,f2 = 1;
              
              wSizeZ = fSizeZ;
              wPosZ = 0.;
              if ( k%2 == 0 ) {
                wSizeX = aSizeZ + gSizeZ;
                wSizeY = fSizeY;
                wPosY  = 0.;
              
                wPosX  = f1*fSizeX - f1*0.5*wSizeX;
              } else if ( k%2 == 1 ) {
                wSizeX = fSizeX;
                wSizeY = aSizeZ + gSizeZ;
                wPosX  = 0.;
              
                wPosY  = f2*fSizeY - f2*0.5*wSizeY;
              }
          }
          Construct();  
          fWrap = ifwrap;
          fCALMaterial = Mat;
          fSizeZ = iSizeZ;
          fVis = iVis;
          fRecordLV = true;
          if( !ifAbsorber ) fCopyNo++;
        }
      }
    }



    return;
}



