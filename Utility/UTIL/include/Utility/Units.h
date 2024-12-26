#ifndef UTILITY_DUNITS_H
#define UTILITY_DUNITS_H

namespace dunits
{
    constexpr double mm = 1.;
    constexpr double cm = 10.*mm;
    constexpr double m = 1000.*mm;

    constexpr double MeV = 1.;
    constexpr double GeV = 1000.*MeV;
    constexpr double TeV = 1000000.*MeV;
    constexpr double keV = 0.001*MeV;
    constexpr double eV = 0.000001*MeV;

    constexpr double Tesla = 1.0;
    constexpr double T = Tesla;
    constexpr double kGauss = 0.1*Tesla;

    #define DSS_DEFINE_UNIT_LITERAL(name)                      \
    constexpr double operator""_##name(long double x) {        \
      return ::dunits::name * x;                               \
    }                                                          \
    constexpr double operator""_##name(unsigned long long x) { \
      return ::dunits::name * x;                               \
    }

    DSS_DEFINE_UNIT_LITERAL(mm)
    DSS_DEFINE_UNIT_LITERAL(cm)
    DSS_DEFINE_UNIT_LITERAL(m)

    DSS_DEFINE_UNIT_LITERAL(GeV)
    DSS_DEFINE_UNIT_LITERAL(TeV)
    DSS_DEFINE_UNIT_LITERAL(MeV)
    DSS_DEFINE_UNIT_LITERAL(keV)
    DSS_DEFINE_UNIT_LITERAL(eV)

    DSS_DEFINE_UNIT_LITERAL(Tesla)
    DSS_DEFINE_UNIT_LITERAL(T)
    DSS_DEFINE_UNIT_LITERAL(kGauss)

    #undef DSS_DEFINE_UNIT_LITERAL

    namespace genfit_to_dss
    {
        constexpr double cm = ::dunits::cm;

        constexpr double GeV = ::dunits::GeV;

        constexpr double kGauss = ::dunits::kGauss;
    }

    namespace dss_to_genfit
    {
        constexpr double mm = 1./::dunits::cm;

        constexpr double MeV = 1./::dunits::GeV;

        constexpr double T = 1./::dunits::kGauss;
    }

    namespace acts_to_dss
    {
        constexpr double mm = ::dunits::mm;

        constexpr double GeV = ::dunits::GeV;

        constexpr double T = 1./0.000299792458;
    }

    namespace dss_to_acts
    {
        constexpr double mm = 1./::dunits::mm;

        constexpr double MeV = 1./::dunits::GeV;

        constexpr double T = 0.000299792458;
    }

    namespace root_to_dss
    {
#ifdef RM_UNIT
        constexpr double ul = ::dunits::mm;
        constexpr double mm = ul;
        constexpr double cm = 10.*ul;
#else
        constexpr double ul = ::dunits::cm;
        constexpr double cm = ul;
        constexpr double mm = 0.1*ul;
#endif

        constexpr double GeV = ::dunits::GeV;
    }

    namespace dss_to_root
    {
#ifdef RM_UNIT
        constexpr double mm = 1./::dunits::mm;
#else
        constexpr double mm = 1./::dunits::cm;
#endif
        constexpr double cm = 10.*mm;

        constexpr double MeV = 1./::dunits::GeV;
    }
}

#endif // UTILITY_DUNITS_H
