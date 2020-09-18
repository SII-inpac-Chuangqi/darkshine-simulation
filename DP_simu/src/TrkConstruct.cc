#include "DP_simu/TrkConstruct.hh"
#include "G4Box.hh"
#include "G4PVPlacement.hh"

#include <iterator>
#include <utility>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrkConstruct::TrkConstruct(G4String TrkName, 
                           G4LogicalVolume* MotherVolume, 
                           G4int CopyNo, 
                           G4bool CheckOverlap)
{
    fTrkName = std::move(TrkName);
    fMotherVolume = MotherVolume;
    fCopyNo = CopyNo;
    fCheckOverlap = CheckOverlap;

    fSizeX = 0.; 
    fSizeY = 0.; 
    fSizeZ = 0.; 
    fPosX = 0.; 
    fPosY = 0.; 
    fPosZ = 0.; 

    fAngle = 0.;
    fZMove = 0.;

    fVis = nullptr;
    fTrkMaterial = nullptr;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrkConstruct::TrkConstruct(const TrkConstruct& in)
{
    fTrkName        = in.fTrkName        ;
    fMotherVolume   = in.fMotherVolume   ;
    fCopyNo         = in.fCopyNo         ;
    fType           = in.fType           ;
    fCheckOverlap   = in.fCheckOverlap   ;
    fSizeX          = in.fSizeX          ;
    fSizeY          = in.fSizeY          ;
    fSizeZ          = in.fSizeZ          ;
    fPosX           = in.fPosX           ;
    fPosY           = in.fPosY           ;
    fPosZ           = in.fPosZ           ;
    fTrkMaterial    = in.fTrkMaterial    ;
    fVis            = in.fVis            ;
    fTrkLVVector    = in.fTrkLVVector    ;
    fAngle          = in.fAngle          ;
    fZMove          = in.fZMove          ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
TrkConstruct::~TrkConstruct()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreeVector TrkConstruct::Construct()
{
    auto TotalSize = G4ThreeVector(0,0,0);
    // check consistency
    if ( !fSizeX || !fSizeY || !fSizeZ ) 
    {
        G4cout<<fTrkName<<" Construction Error: at least size of one dimension is zero."<<G4endl; 
        return TotalSize;
    }

    // Calculate Rotation Matrix
    auto HepRot = new G4RotationMatrix();
    //HepRot->set( CLHEP::Hep3Vector ( 1., 0., 1.), fAngle );
    HepRot->rotateZ( fAngle );

    //G4cout<<"Angle: "<<fAngle<<", "<<   HepRot->thetaZ () <<G4endl;

    // Placement of Tracker
    auto pos = G4ThreeVector(fPosX, fPosY, fPosZ + fZMove);

    auto box = new G4Box(fTrkName+"_Box" , fSizeX/2., fSizeY/2., fSizeZ/2. );
    auto boxLV = new G4LogicalVolume(box, fTrkMaterial, fTrkName+"_LV", nullptr,nullptr,nullptr);
    new G4PVPlacement( HepRot, pos, boxLV, fTrkName+"_PV", fMotherVolume, false, fCopyNo, fCheckOverlap);

    fTrkLVVector.push_back(boxLV);

    if ( fVis ) 
    {
        fVis->SetVisibility(true); 
        boxLV->SetVisAttributes(fVis);
    }
    else boxLV->SetVisAttributes(G4VisAttributes::GetInvisible());

    TotalSize = G4ThreeVector(fSizeX, fSizeY, fSizeZ);

    return TotalSize;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreeVector TrkConstruct::LinearPlacement(G4int zNo, G4ThreeVector* SizeVec, G4ThreeVector* PosVec)
{
    auto TotalSize = G4ThreeVector(0,0,0);

    for (int k = 0; k < zNo; k++) 
    {
        auto CurSizeVec = *(SizeVec+k);
        auto CurPosVec  = *(PosVec+k);

        SetSizeXYZ(CurSizeVec); 
        SetPosXYZ(CurPosVec); 

        Construct();  
        
        fCopyNo++;
    }

    return TotalSize;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

