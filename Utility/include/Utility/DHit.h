//
// Created by Zhang Yulei on 9/18/20.
//

#ifndef DSIMU_DHIT_H
#define DSIMU_DHIT_H

enum DetectorType {
    TagTrk, RecTrk, ECAL_Center, ECAL_Outer, HCAL
};

class DHit {
public:
    // Constructor and Destructor
    DHit() = default;

    DHit(const DHit &rhs) {
        *this = rhs;
    }

    virtual ~DHit() = default;

    bool operator==(const DHit &rhs) const {
        return id == rhs.id &&
               CellID == rhs.CellID &&
               Detector == rhs.Detector &&
               X == rhs.X &&
               Y == rhs.Y &&
               Z == rhs.Z &&
               T == rhs.T &&
               E == rhs.E;
    }

    bool operator!=(const DHit &rhs) const {
        return !(rhs == *this);
    }

    DHit &operator=(const DHit &rhs) {
        if (&rhs == this) { return *this; }
        id = rhs.id;
        CellID = rhs.CellID;
        Detector = rhs.Detector;
        X = rhs.X;
        Y = rhs.Y;
        Z = rhs.Z;
        T = rhs.T;
        E = rhs.E;

        return *this;
    }

    // Get Methods
    int getId() const {
        return id;
    }

    int getCellId() const {
        return CellID;
    }

    double getX() const {
        return X;
    }

    double getY() const {
        return Y;
    }

    double getZ() const {
        return Z;
    }

    double getT() const {
        return T;
    }

    double getE() const {
        return E;
    }

    DetectorType getDetector() const {
        return Detector;
    }

    // Set Methods
    void setId(int ID) {
        DHit::id = ID;
    }

    void setCellId(int cellId) {
        CellID = cellId;
    }

    void setX(double x) {
        X = x;
    }

    void setY(double y) {
        Y = y;
    }

    void setZ(double z) {
        Z = z;
    }

    void setT(double t) {
        T = t;
    }

    void setE(double e) {
        E = e;
    }

    void setDetector(DetectorType detector) {
        Detector = detector;
    }

protected:
    int id{0};
    int CellID{0};

    DetectorType Detector;

    double X{0.};
    double Y{0.};
    double Z{0.};
    double T{0.};
    double E{0.};
};

#endif //DSIMU_DHIT_H
