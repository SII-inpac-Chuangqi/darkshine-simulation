//
// Created by Zhang Yulei on 9/18/20.
//

#ifndef DSIMU_DHIT_H
#define DSIMU_DHIT_H

class DHit {
public:
    // Constructor and Destructor
    DHit() = default;

    DHit(const DHit &rhs) {
        id = rhs.id ;
        CellID = rhs.CellID ;
        X = rhs.X ;
        Y = rhs.Y ;
        Z = rhs.Z ;
        T = rhs.T ;
        E = rhs.E ;
    }

    virtual ~DHit() = default;

    // Operators
    bool operator==(const DHit &rhs) const {
        return id == rhs.id &&
               CellID == rhs.CellID &&
               X == rhs.X &&
               Y == rhs.Y &&
               Z == rhs.Z &&
               T == rhs.T &&
               E == rhs.E;
    }

    bool operator!=(const DHit &rhs) const {
        return !(rhs == *this);
    }

    DHit &operator=(const DHit &rhs ) {
        if (&rhs == this) { return *this; }
        id = rhs.id ;
        CellID = rhs.CellID ;
        X = rhs.X ;
        Y = rhs.Y ;
        Z = rhs.Z ;
        T = rhs.T ;
        E = rhs.E ;

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

protected:
    int id;
    int CellID;

    double X;
    double Y;
    double Z;
    double T;
    double E;
};

#endif //DSIMU_DHIT_H
