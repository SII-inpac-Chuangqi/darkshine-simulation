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

#include <math.h>
#include <iostream>

#include "TMultiDimFit.h"

#include "Utility/Units.h"

#include "MapField.h"

namespace genfit
{

//................................................................................//
//public:
//................................................................................//
//Constructor & destructor
    MapField::MapField(const DMagnet &Bx, const DMagnet &By, const DMagnet &Bz, double unit) : unit_(unit)
    {
        Bx_ = &(const_cast<DMagnet&>(Bx));
        By_ = &(const_cast<DMagnet&>(By));
        Bz_ = &(const_cast<DMagnet&>(Bz));
    }

//................................................................................//
//Set

//................................................................................//
//Get    
    TVector3 MapField::get(const TVector3& pos) const
    {
        double Bx, By, Bz;
        this->get(pos.X(), pos.Y(), pos.Z(),
                  Bx,      By,      Bz);

        TVector3 field(Bx, By, Bz);
        return field;
    }

    void MapField::get(const double& x, const double& y, const double& z,
                       double& Bx,      double& By,      double& Bz) const
    {
        using namespace dunits;

        if(!Bx_ || !By_ || !Bz_)
        {
            std::cout << "[WARNING] ==> No manget found" << std::endl;
            Bx = INFINITY;
            By = INFINITY;
            Bz = INFINITY;
        }

        Bx = Bx_->GetField(x * genfit_to_dss::cm, y * genfit_to_dss::cm, z * genfit_to_dss::cm)*unit_;
        By = By_->GetField(x * genfit_to_dss::cm, y * genfit_to_dss::cm, z * genfit_to_dss::cm)*unit_;
        Bz = Bz_->GetField(x * genfit_to_dss::cm, y * genfit_to_dss::cm, z * genfit_to_dss::cm)*unit_;
    }

//................................................................................//
//private:

}
//End of namespace genfit
