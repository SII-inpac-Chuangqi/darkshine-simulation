/*  ................................................................................
 *  Copyright 2008-2010, Technische Universitaet Muenchen,
 *  Authors: Christian Hoeppner & Sebastian Neubert & Johannes Rauch

 *  This file is part of GENFIT.

 *  GENFIT is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published
 *  by the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.

 *  GENFIT is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.

 *  You should have received a copy of the GNU Lesser General Public License
 *  along with GENFIT.  If not, see <http://www.gnu.org/licenses/>.
 *  ................................................................................ */

/*  ................................................................................
 *  Developed by Zhu Yifan @Dark Shine
 *  ................................................................................ */

#ifndef genfit_MapField_h
#define genfit_MapField_h

#include "TH3D.h"
#include "TMultiDimFit.h"

#include "AbsBField.h"

#include "DMagnet.h"

namespace genfit
{
    enum {Tesla = 10, kGauss = 1};

    class MapField : public AbsBField
    {
    public:
//................................................................................//
//Constructor & destructor
        MapField() {}
        MapField(const DMagnet &Bx, const DMagnet &By, const DMagnet &Bz, int unit = kGauss);
        ~MapField() {}

//................................................................................//
//Set

//................................................................................//
//Get
        TVector3 get(const TVector3& pos) const;
        void get(const double& posX, const double& posY, const double& posZ,
                 double& Bx, double& By, double& Bz) const;
 
    private:

        DMagnet *Bx_;
        DMagnet *By_;
        DMagnet *Bz_;

        int unit_;
    };

}
//End of namespace genfit

#endif //genfit_MapField_h
