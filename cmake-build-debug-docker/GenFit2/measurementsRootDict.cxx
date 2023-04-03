// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME measurementsRootDict
#define R__NO_DEPRECATION

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Header files passed as explicit arguments
#include "/tmp/source/GenFit2/measurements/include/HMatrixPhi.h"
#include "/tmp/source/GenFit2/measurements/include/HMatrixU.h"
#include "/tmp/source/GenFit2/measurements/include/HMatrixUnit.h"
#include "/tmp/source/GenFit2/measurements/include/HMatrixV.h"
#include "/tmp/source/GenFit2/measurements/include/HMatrixUV.h"
#include "/tmp/source/GenFit2/measurements/include/FullMeasurement.h"
#include "/tmp/source/GenFit2/measurements/include/PlanarMeasurement.h"
#include "/tmp/source/GenFit2/measurements/include/ProlateSpacepointMeasurement.h"
#include "/tmp/source/GenFit2/measurements/include/SpacepointMeasurement.h"
#include "/tmp/source/GenFit2/measurements/include/WireMeasurement.h"
#include "/tmp/source/GenFit2/measurements/include/WireMeasurementNew.h"
#include "/tmp/source/GenFit2/measurements/include/WirePointMeasurement.h"
#include "/tmp/source/GenFit2/measurements/include/WireTrackCandHit.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace genfit {
   namespace ROOTDict {
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance();
      static TClass *genfit_Dictionary();

      // Function generating the singleton type initializer
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance()
      {
         static ::ROOT::TGenericClassInfo 
            instance("genfit", 0 /*version*/, "AbsHMatrix.h", 31,
                     ::ROOT::Internal::DefineBehavior((void*)nullptr,(void*)nullptr),
                     &genfit_Dictionary, 0);
         return &instance;
      }
      // Insure that the inline function is _not_ optimized away by the compiler
      ::ROOT::TGenericClassInfo *(*_R__UNIQUE_DICT_(InitFunctionKeeper))() = &GenerateInitInstance;  
      // Static variable to force the class initialization
      static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstance(); R__UseDummy(_R__UNIQUE_DICT_(Init));

      // Dictionary for non-ClassDef classes
      static TClass *genfit_Dictionary() {
         return GenerateInitInstance()->GetClass();
      }

   }
}

namespace ROOT {
   static void *new_genfitcLcLHMatrixPhi(void *p = nullptr);
   static void *newArray_genfitcLcLHMatrixPhi(Long_t size, void *p);
   static void delete_genfitcLcLHMatrixPhi(void *p);
   static void deleteArray_genfitcLcLHMatrixPhi(void *p);
   static void destruct_genfitcLcLHMatrixPhi(void *p);
   static void streamer_genfitcLcLHMatrixPhi(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::genfit::HMatrixPhi*)
   {
      ::genfit::HMatrixPhi *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::genfit::HMatrixPhi >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("genfit::HMatrixPhi", ::genfit::HMatrixPhi::Class_Version(), "HMatrixPhi.h", 37,
                  typeid(::genfit::HMatrixPhi), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::genfit::HMatrixPhi::Dictionary, isa_proxy, 17,
                  sizeof(::genfit::HMatrixPhi) );
      instance.SetNew(&new_genfitcLcLHMatrixPhi);
      instance.SetNewArray(&newArray_genfitcLcLHMatrixPhi);
      instance.SetDelete(&delete_genfitcLcLHMatrixPhi);
      instance.SetDeleteArray(&deleteArray_genfitcLcLHMatrixPhi);
      instance.SetDestructor(&destruct_genfitcLcLHMatrixPhi);
      instance.SetStreamerFunc(&streamer_genfitcLcLHMatrixPhi);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::genfit::HMatrixPhi*)
   {
      return GenerateInitInstanceLocal((::genfit::HMatrixPhi*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::genfit::HMatrixPhi*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_genfitcLcLHMatrixU(void *p = nullptr);
   static void *newArray_genfitcLcLHMatrixU(Long_t size, void *p);
   static void delete_genfitcLcLHMatrixU(void *p);
   static void deleteArray_genfitcLcLHMatrixU(void *p);
   static void destruct_genfitcLcLHMatrixU(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::genfit::HMatrixU*)
   {
      ::genfit::HMatrixU *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::genfit::HMatrixU >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("genfit::HMatrixU", ::genfit::HMatrixU::Class_Version(), "HMatrixU.h", 37,
                  typeid(::genfit::HMatrixU), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::genfit::HMatrixU::Dictionary, isa_proxy, 4,
                  sizeof(::genfit::HMatrixU) );
      instance.SetNew(&new_genfitcLcLHMatrixU);
      instance.SetNewArray(&newArray_genfitcLcLHMatrixU);
      instance.SetDelete(&delete_genfitcLcLHMatrixU);
      instance.SetDeleteArray(&deleteArray_genfitcLcLHMatrixU);
      instance.SetDestructor(&destruct_genfitcLcLHMatrixU);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::genfit::HMatrixU*)
   {
      return GenerateInitInstanceLocal((::genfit::HMatrixU*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::genfit::HMatrixU*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_genfitcLcLHMatrixUnit(void *p = nullptr);
   static void *newArray_genfitcLcLHMatrixUnit(Long_t size, void *p);
   static void delete_genfitcLcLHMatrixUnit(void *p);
   static void deleteArray_genfitcLcLHMatrixUnit(void *p);
   static void destruct_genfitcLcLHMatrixUnit(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::genfit::HMatrixUnit*)
   {
      ::genfit::HMatrixUnit *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::genfit::HMatrixUnit >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("genfit::HMatrixUnit", ::genfit::HMatrixUnit::Class_Version(), "HMatrixUnit.h", 40,
                  typeid(::genfit::HMatrixUnit), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::genfit::HMatrixUnit::Dictionary, isa_proxy, 4,
                  sizeof(::genfit::HMatrixUnit) );
      instance.SetNew(&new_genfitcLcLHMatrixUnit);
      instance.SetNewArray(&newArray_genfitcLcLHMatrixUnit);
      instance.SetDelete(&delete_genfitcLcLHMatrixUnit);
      instance.SetDeleteArray(&deleteArray_genfitcLcLHMatrixUnit);
      instance.SetDestructor(&destruct_genfitcLcLHMatrixUnit);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::genfit::HMatrixUnit*)
   {
      return GenerateInitInstanceLocal((::genfit::HMatrixUnit*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::genfit::HMatrixUnit*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_genfitcLcLHMatrixV(void *p = nullptr);
   static void *newArray_genfitcLcLHMatrixV(Long_t size, void *p);
   static void delete_genfitcLcLHMatrixV(void *p);
   static void deleteArray_genfitcLcLHMatrixV(void *p);
   static void destruct_genfitcLcLHMatrixV(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::genfit::HMatrixV*)
   {
      ::genfit::HMatrixV *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::genfit::HMatrixV >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("genfit::HMatrixV", ::genfit::HMatrixV::Class_Version(), "HMatrixV.h", 37,
                  typeid(::genfit::HMatrixV), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::genfit::HMatrixV::Dictionary, isa_proxy, 4,
                  sizeof(::genfit::HMatrixV) );
      instance.SetNew(&new_genfitcLcLHMatrixV);
      instance.SetNewArray(&newArray_genfitcLcLHMatrixV);
      instance.SetDelete(&delete_genfitcLcLHMatrixV);
      instance.SetDeleteArray(&deleteArray_genfitcLcLHMatrixV);
      instance.SetDestructor(&destruct_genfitcLcLHMatrixV);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::genfit::HMatrixV*)
   {
      return GenerateInitInstanceLocal((::genfit::HMatrixV*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::genfit::HMatrixV*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_genfitcLcLHMatrixUV(void *p = nullptr);
   static void *newArray_genfitcLcLHMatrixUV(Long_t size, void *p);
   static void delete_genfitcLcLHMatrixUV(void *p);
   static void deleteArray_genfitcLcLHMatrixUV(void *p);
   static void destruct_genfitcLcLHMatrixUV(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::genfit::HMatrixUV*)
   {
      ::genfit::HMatrixUV *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::genfit::HMatrixUV >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("genfit::HMatrixUV", ::genfit::HMatrixUV::Class_Version(), "HMatrixUV.h", 39,
                  typeid(::genfit::HMatrixUV), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::genfit::HMatrixUV::Dictionary, isa_proxy, 4,
                  sizeof(::genfit::HMatrixUV) );
      instance.SetNew(&new_genfitcLcLHMatrixUV);
      instance.SetNewArray(&newArray_genfitcLcLHMatrixUV);
      instance.SetDelete(&delete_genfitcLcLHMatrixUV);
      instance.SetDeleteArray(&deleteArray_genfitcLcLHMatrixUV);
      instance.SetDestructor(&destruct_genfitcLcLHMatrixUV);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::genfit::HMatrixUV*)
   {
      return GenerateInitInstanceLocal((::genfit::HMatrixUV*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::genfit::HMatrixUV*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_genfitcLcLFullMeasurement(void *p = nullptr);
   static void *newArray_genfitcLcLFullMeasurement(Long_t size, void *p);
   static void delete_genfitcLcLFullMeasurement(void *p);
   static void deleteArray_genfitcLcLFullMeasurement(void *p);
   static void destruct_genfitcLcLFullMeasurement(void *p);
   static void streamer_genfitcLcLFullMeasurement(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::genfit::FullMeasurement*)
   {
      ::genfit::FullMeasurement *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::genfit::FullMeasurement >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("genfit::FullMeasurement", ::genfit::FullMeasurement::Class_Version(), "FullMeasurement.h", 42,
                  typeid(::genfit::FullMeasurement), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::genfit::FullMeasurement::Dictionary, isa_proxy, 17,
                  sizeof(::genfit::FullMeasurement) );
      instance.SetNew(&new_genfitcLcLFullMeasurement);
      instance.SetNewArray(&newArray_genfitcLcLFullMeasurement);
      instance.SetDelete(&delete_genfitcLcLFullMeasurement);
      instance.SetDeleteArray(&deleteArray_genfitcLcLFullMeasurement);
      instance.SetDestructor(&destruct_genfitcLcLFullMeasurement);
      instance.SetStreamerFunc(&streamer_genfitcLcLFullMeasurement);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::genfit::FullMeasurement*)
   {
      return GenerateInitInstanceLocal((::genfit::FullMeasurement*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::genfit::FullMeasurement*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_genfitcLcLPlanarMeasurement(void *p = nullptr);
   static void *newArray_genfitcLcLPlanarMeasurement(Long_t size, void *p);
   static void delete_genfitcLcLPlanarMeasurement(void *p);
   static void deleteArray_genfitcLcLPlanarMeasurement(void *p);
   static void destruct_genfitcLcLPlanarMeasurement(void *p);
   static void streamer_genfitcLcLPlanarMeasurement(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::genfit::PlanarMeasurement*)
   {
      ::genfit::PlanarMeasurement *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::genfit::PlanarMeasurement >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("genfit::PlanarMeasurement", ::genfit::PlanarMeasurement::Class_Version(), "PlanarMeasurement.h", 44,
                  typeid(::genfit::PlanarMeasurement), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::genfit::PlanarMeasurement::Dictionary, isa_proxy, 17,
                  sizeof(::genfit::PlanarMeasurement) );
      instance.SetNew(&new_genfitcLcLPlanarMeasurement);
      instance.SetNewArray(&newArray_genfitcLcLPlanarMeasurement);
      instance.SetDelete(&delete_genfitcLcLPlanarMeasurement);
      instance.SetDeleteArray(&deleteArray_genfitcLcLPlanarMeasurement);
      instance.SetDestructor(&destruct_genfitcLcLPlanarMeasurement);
      instance.SetStreamerFunc(&streamer_genfitcLcLPlanarMeasurement);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::genfit::PlanarMeasurement*)
   {
      return GenerateInitInstanceLocal((::genfit::PlanarMeasurement*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::genfit::PlanarMeasurement*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_genfitcLcLSpacepointMeasurement(void *p = nullptr);
   static void *newArray_genfitcLcLSpacepointMeasurement(Long_t size, void *p);
   static void delete_genfitcLcLSpacepointMeasurement(void *p);
   static void deleteArray_genfitcLcLSpacepointMeasurement(void *p);
   static void destruct_genfitcLcLSpacepointMeasurement(void *p);
   static void streamer_genfitcLcLSpacepointMeasurement(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::genfit::SpacepointMeasurement*)
   {
      ::genfit::SpacepointMeasurement *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::genfit::SpacepointMeasurement >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("genfit::SpacepointMeasurement", ::genfit::SpacepointMeasurement::Class_Version(), "SpacepointMeasurement.h", 46,
                  typeid(::genfit::SpacepointMeasurement), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::genfit::SpacepointMeasurement::Dictionary, isa_proxy, 17,
                  sizeof(::genfit::SpacepointMeasurement) );
      instance.SetNew(&new_genfitcLcLSpacepointMeasurement);
      instance.SetNewArray(&newArray_genfitcLcLSpacepointMeasurement);
      instance.SetDelete(&delete_genfitcLcLSpacepointMeasurement);
      instance.SetDeleteArray(&deleteArray_genfitcLcLSpacepointMeasurement);
      instance.SetDestructor(&destruct_genfitcLcLSpacepointMeasurement);
      instance.SetStreamerFunc(&streamer_genfitcLcLSpacepointMeasurement);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::genfit::SpacepointMeasurement*)
   {
      return GenerateInitInstanceLocal((::genfit::SpacepointMeasurement*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::genfit::SpacepointMeasurement*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_genfitcLcLProlateSpacepointMeasurement(void *p = nullptr);
   static void *newArray_genfitcLcLProlateSpacepointMeasurement(Long_t size, void *p);
   static void delete_genfitcLcLProlateSpacepointMeasurement(void *p);
   static void deleteArray_genfitcLcLProlateSpacepointMeasurement(void *p);
   static void destruct_genfitcLcLProlateSpacepointMeasurement(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::genfit::ProlateSpacepointMeasurement*)
   {
      ::genfit::ProlateSpacepointMeasurement *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::genfit::ProlateSpacepointMeasurement >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("genfit::ProlateSpacepointMeasurement", ::genfit::ProlateSpacepointMeasurement::Class_Version(), "ProlateSpacepointMeasurement.h", 46,
                  typeid(::genfit::ProlateSpacepointMeasurement), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::genfit::ProlateSpacepointMeasurement::Dictionary, isa_proxy, 4,
                  sizeof(::genfit::ProlateSpacepointMeasurement) );
      instance.SetNew(&new_genfitcLcLProlateSpacepointMeasurement);
      instance.SetNewArray(&newArray_genfitcLcLProlateSpacepointMeasurement);
      instance.SetDelete(&delete_genfitcLcLProlateSpacepointMeasurement);
      instance.SetDeleteArray(&deleteArray_genfitcLcLProlateSpacepointMeasurement);
      instance.SetDestructor(&destruct_genfitcLcLProlateSpacepointMeasurement);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::genfit::ProlateSpacepointMeasurement*)
   {
      return GenerateInitInstanceLocal((::genfit::ProlateSpacepointMeasurement*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::genfit::ProlateSpacepointMeasurement*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_genfitcLcLWireMeasurement(void *p = nullptr);
   static void *newArray_genfitcLcLWireMeasurement(Long_t size, void *p);
   static void delete_genfitcLcLWireMeasurement(void *p);
   static void deleteArray_genfitcLcLWireMeasurement(void *p);
   static void destruct_genfitcLcLWireMeasurement(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::genfit::WireMeasurement*)
   {
      ::genfit::WireMeasurement *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::genfit::WireMeasurement >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("genfit::WireMeasurement", ::genfit::WireMeasurement::Class_Version(), "WireMeasurement.h", 52,
                  typeid(::genfit::WireMeasurement), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::genfit::WireMeasurement::Dictionary, isa_proxy, 4,
                  sizeof(::genfit::WireMeasurement) );
      instance.SetNew(&new_genfitcLcLWireMeasurement);
      instance.SetNewArray(&newArray_genfitcLcLWireMeasurement);
      instance.SetDelete(&delete_genfitcLcLWireMeasurement);
      instance.SetDeleteArray(&deleteArray_genfitcLcLWireMeasurement);
      instance.SetDestructor(&destruct_genfitcLcLWireMeasurement);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::genfit::WireMeasurement*)
   {
      return GenerateInitInstanceLocal((::genfit::WireMeasurement*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::genfit::WireMeasurement*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_genfitcLcLWireMeasurementNew(void *p = nullptr);
   static void *newArray_genfitcLcLWireMeasurementNew(Long_t size, void *p);
   static void delete_genfitcLcLWireMeasurementNew(void *p);
   static void deleteArray_genfitcLcLWireMeasurementNew(void *p);
   static void destruct_genfitcLcLWireMeasurementNew(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::genfit::WireMeasurementNew*)
   {
      ::genfit::WireMeasurementNew *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::genfit::WireMeasurementNew >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("genfit::WireMeasurementNew", ::genfit::WireMeasurementNew::Class_Version(), "WireMeasurementNew.h", 56,
                  typeid(::genfit::WireMeasurementNew), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::genfit::WireMeasurementNew::Dictionary, isa_proxy, 4,
                  sizeof(::genfit::WireMeasurementNew) );
      instance.SetNew(&new_genfitcLcLWireMeasurementNew);
      instance.SetNewArray(&newArray_genfitcLcLWireMeasurementNew);
      instance.SetDelete(&delete_genfitcLcLWireMeasurementNew);
      instance.SetDeleteArray(&deleteArray_genfitcLcLWireMeasurementNew);
      instance.SetDestructor(&destruct_genfitcLcLWireMeasurementNew);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::genfit::WireMeasurementNew*)
   {
      return GenerateInitInstanceLocal((::genfit::WireMeasurementNew*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::genfit::WireMeasurementNew*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_genfitcLcLWirePointMeasurement(void *p = nullptr);
   static void *newArray_genfitcLcLWirePointMeasurement(Long_t size, void *p);
   static void delete_genfitcLcLWirePointMeasurement(void *p);
   static void deleteArray_genfitcLcLWirePointMeasurement(void *p);
   static void destruct_genfitcLcLWirePointMeasurement(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::genfit::WirePointMeasurement*)
   {
      ::genfit::WirePointMeasurement *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::genfit::WirePointMeasurement >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("genfit::WirePointMeasurement", ::genfit::WirePointMeasurement::Class_Version(), "WirePointMeasurement.h", 51,
                  typeid(::genfit::WirePointMeasurement), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::genfit::WirePointMeasurement::Dictionary, isa_proxy, 4,
                  sizeof(::genfit::WirePointMeasurement) );
      instance.SetNew(&new_genfitcLcLWirePointMeasurement);
      instance.SetNewArray(&newArray_genfitcLcLWirePointMeasurement);
      instance.SetDelete(&delete_genfitcLcLWirePointMeasurement);
      instance.SetDeleteArray(&deleteArray_genfitcLcLWirePointMeasurement);
      instance.SetDestructor(&destruct_genfitcLcLWirePointMeasurement);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::genfit::WirePointMeasurement*)
   {
      return GenerateInitInstanceLocal((::genfit::WirePointMeasurement*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::genfit::WirePointMeasurement*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_genfitcLcLWireTrackCandHit(void *p = nullptr);
   static void *newArray_genfitcLcLWireTrackCandHit(Long_t size, void *p);
   static void delete_genfitcLcLWireTrackCandHit(void *p);
   static void deleteArray_genfitcLcLWireTrackCandHit(void *p);
   static void destruct_genfitcLcLWireTrackCandHit(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::genfit::WireTrackCandHit*)
   {
      ::genfit::WireTrackCandHit *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::genfit::WireTrackCandHit >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("genfit::WireTrackCandHit", ::genfit::WireTrackCandHit::Class_Version(), "WireTrackCandHit.h", 34,
                  typeid(::genfit::WireTrackCandHit), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::genfit::WireTrackCandHit::Dictionary, isa_proxy, 4,
                  sizeof(::genfit::WireTrackCandHit) );
      instance.SetNew(&new_genfitcLcLWireTrackCandHit);
      instance.SetNewArray(&newArray_genfitcLcLWireTrackCandHit);
      instance.SetDelete(&delete_genfitcLcLWireTrackCandHit);
      instance.SetDeleteArray(&deleteArray_genfitcLcLWireTrackCandHit);
      instance.SetDestructor(&destruct_genfitcLcLWireTrackCandHit);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::genfit::WireTrackCandHit*)
   {
      return GenerateInitInstanceLocal((::genfit::WireTrackCandHit*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::genfit::WireTrackCandHit*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace genfit {
//______________________________________________________________________________
atomic_TClass_ptr HMatrixPhi::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *HMatrixPhi::Class_Name()
{
   return "genfit::HMatrixPhi";
}

//______________________________________________________________________________
const char *HMatrixPhi::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::HMatrixPhi*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int HMatrixPhi::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::HMatrixPhi*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *HMatrixPhi::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::HMatrixPhi*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *HMatrixPhi::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::HMatrixPhi*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace genfit
namespace genfit {
//______________________________________________________________________________
atomic_TClass_ptr HMatrixU::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *HMatrixU::Class_Name()
{
   return "genfit::HMatrixU";
}

//______________________________________________________________________________
const char *HMatrixU::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::HMatrixU*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int HMatrixU::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::HMatrixU*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *HMatrixU::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::HMatrixU*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *HMatrixU::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::HMatrixU*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace genfit
namespace genfit {
//______________________________________________________________________________
atomic_TClass_ptr HMatrixUnit::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *HMatrixUnit::Class_Name()
{
   return "genfit::HMatrixUnit";
}

//______________________________________________________________________________
const char *HMatrixUnit::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::HMatrixUnit*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int HMatrixUnit::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::HMatrixUnit*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *HMatrixUnit::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::HMatrixUnit*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *HMatrixUnit::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::HMatrixUnit*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace genfit
namespace genfit {
//______________________________________________________________________________
atomic_TClass_ptr HMatrixV::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *HMatrixV::Class_Name()
{
   return "genfit::HMatrixV";
}

//______________________________________________________________________________
const char *HMatrixV::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::HMatrixV*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int HMatrixV::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::HMatrixV*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *HMatrixV::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::HMatrixV*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *HMatrixV::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::HMatrixV*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace genfit
namespace genfit {
//______________________________________________________________________________
atomic_TClass_ptr HMatrixUV::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *HMatrixUV::Class_Name()
{
   return "genfit::HMatrixUV";
}

//______________________________________________________________________________
const char *HMatrixUV::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::HMatrixUV*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int HMatrixUV::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::HMatrixUV*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *HMatrixUV::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::HMatrixUV*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *HMatrixUV::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::HMatrixUV*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace genfit
namespace genfit {
//______________________________________________________________________________
atomic_TClass_ptr FullMeasurement::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *FullMeasurement::Class_Name()
{
   return "genfit::FullMeasurement";
}

//______________________________________________________________________________
const char *FullMeasurement::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::FullMeasurement*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int FullMeasurement::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::FullMeasurement*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *FullMeasurement::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::FullMeasurement*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *FullMeasurement::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::FullMeasurement*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace genfit
namespace genfit {
//______________________________________________________________________________
atomic_TClass_ptr PlanarMeasurement::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *PlanarMeasurement::Class_Name()
{
   return "genfit::PlanarMeasurement";
}

//______________________________________________________________________________
const char *PlanarMeasurement::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::PlanarMeasurement*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int PlanarMeasurement::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::PlanarMeasurement*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *PlanarMeasurement::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::PlanarMeasurement*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *PlanarMeasurement::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::PlanarMeasurement*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace genfit
namespace genfit {
//______________________________________________________________________________
atomic_TClass_ptr SpacepointMeasurement::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *SpacepointMeasurement::Class_Name()
{
   return "genfit::SpacepointMeasurement";
}

//______________________________________________________________________________
const char *SpacepointMeasurement::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::SpacepointMeasurement*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int SpacepointMeasurement::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::SpacepointMeasurement*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *SpacepointMeasurement::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::SpacepointMeasurement*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *SpacepointMeasurement::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::SpacepointMeasurement*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace genfit
namespace genfit {
//______________________________________________________________________________
atomic_TClass_ptr ProlateSpacepointMeasurement::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *ProlateSpacepointMeasurement::Class_Name()
{
   return "genfit::ProlateSpacepointMeasurement";
}

//______________________________________________________________________________
const char *ProlateSpacepointMeasurement::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::ProlateSpacepointMeasurement*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int ProlateSpacepointMeasurement::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::ProlateSpacepointMeasurement*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ProlateSpacepointMeasurement::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::ProlateSpacepointMeasurement*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ProlateSpacepointMeasurement::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::ProlateSpacepointMeasurement*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace genfit
namespace genfit {
//______________________________________________________________________________
atomic_TClass_ptr WireMeasurement::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *WireMeasurement::Class_Name()
{
   return "genfit::WireMeasurement";
}

//______________________________________________________________________________
const char *WireMeasurement::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::WireMeasurement*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int WireMeasurement::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::WireMeasurement*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *WireMeasurement::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::WireMeasurement*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *WireMeasurement::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::WireMeasurement*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace genfit
namespace genfit {
//______________________________________________________________________________
atomic_TClass_ptr WireMeasurementNew::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *WireMeasurementNew::Class_Name()
{
   return "genfit::WireMeasurementNew";
}

//______________________________________________________________________________
const char *WireMeasurementNew::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::WireMeasurementNew*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int WireMeasurementNew::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::WireMeasurementNew*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *WireMeasurementNew::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::WireMeasurementNew*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *WireMeasurementNew::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::WireMeasurementNew*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace genfit
namespace genfit {
//______________________________________________________________________________
atomic_TClass_ptr WirePointMeasurement::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *WirePointMeasurement::Class_Name()
{
   return "genfit::WirePointMeasurement";
}

//______________________________________________________________________________
const char *WirePointMeasurement::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::WirePointMeasurement*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int WirePointMeasurement::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::WirePointMeasurement*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *WirePointMeasurement::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::WirePointMeasurement*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *WirePointMeasurement::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::WirePointMeasurement*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace genfit
namespace genfit {
//______________________________________________________________________________
atomic_TClass_ptr WireTrackCandHit::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *WireTrackCandHit::Class_Name()
{
   return "genfit::WireTrackCandHit";
}

//______________________________________________________________________________
const char *WireTrackCandHit::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::WireTrackCandHit*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int WireTrackCandHit::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::WireTrackCandHit*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *WireTrackCandHit::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::WireTrackCandHit*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *WireTrackCandHit::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::WireTrackCandHit*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace genfit
namespace ROOT {
   // Wrappers around operator new
   static void *new_genfitcLcLHMatrixPhi(void *p) {
      return  p ? new(p) ::genfit::HMatrixPhi : new ::genfit::HMatrixPhi;
   }
   static void *newArray_genfitcLcLHMatrixPhi(Long_t nElements, void *p) {
      return p ? new(p) ::genfit::HMatrixPhi[nElements] : new ::genfit::HMatrixPhi[nElements];
   }
   // Wrapper around operator delete
   static void delete_genfitcLcLHMatrixPhi(void *p) {
      delete ((::genfit::HMatrixPhi*)p);
   }
   static void deleteArray_genfitcLcLHMatrixPhi(void *p) {
      delete [] ((::genfit::HMatrixPhi*)p);
   }
   static void destruct_genfitcLcLHMatrixPhi(void *p) {
      typedef ::genfit::HMatrixPhi current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_genfitcLcLHMatrixPhi(TBuffer &buf, void *obj) {
      ((::genfit::HMatrixPhi*)obj)->::genfit::HMatrixPhi::Streamer(buf);
   }
} // end of namespace ROOT for class ::genfit::HMatrixPhi

namespace genfit {
//______________________________________________________________________________
void HMatrixU::Streamer(TBuffer &R__b)
{
   // Stream an object of class genfit::HMatrixU.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(genfit::HMatrixU::Class(),this);
   } else {
      R__b.WriteClassBuffer(genfit::HMatrixU::Class(),this);
   }
}

} // namespace genfit
namespace ROOT {
   // Wrappers around operator new
   static void *new_genfitcLcLHMatrixU(void *p) {
      return  p ? new(p) ::genfit::HMatrixU : new ::genfit::HMatrixU;
   }
   static void *newArray_genfitcLcLHMatrixU(Long_t nElements, void *p) {
      return p ? new(p) ::genfit::HMatrixU[nElements] : new ::genfit::HMatrixU[nElements];
   }
   // Wrapper around operator delete
   static void delete_genfitcLcLHMatrixU(void *p) {
      delete ((::genfit::HMatrixU*)p);
   }
   static void deleteArray_genfitcLcLHMatrixU(void *p) {
      delete [] ((::genfit::HMatrixU*)p);
   }
   static void destruct_genfitcLcLHMatrixU(void *p) {
      typedef ::genfit::HMatrixU current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::genfit::HMatrixU

namespace genfit {
//______________________________________________________________________________
void HMatrixUnit::Streamer(TBuffer &R__b)
{
   // Stream an object of class genfit::HMatrixUnit.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(genfit::HMatrixUnit::Class(),this);
   } else {
      R__b.WriteClassBuffer(genfit::HMatrixUnit::Class(),this);
   }
}

} // namespace genfit
namespace ROOT {
   // Wrappers around operator new
   static void *new_genfitcLcLHMatrixUnit(void *p) {
      return  p ? new(p) ::genfit::HMatrixUnit : new ::genfit::HMatrixUnit;
   }
   static void *newArray_genfitcLcLHMatrixUnit(Long_t nElements, void *p) {
      return p ? new(p) ::genfit::HMatrixUnit[nElements] : new ::genfit::HMatrixUnit[nElements];
   }
   // Wrapper around operator delete
   static void delete_genfitcLcLHMatrixUnit(void *p) {
      delete ((::genfit::HMatrixUnit*)p);
   }
   static void deleteArray_genfitcLcLHMatrixUnit(void *p) {
      delete [] ((::genfit::HMatrixUnit*)p);
   }
   static void destruct_genfitcLcLHMatrixUnit(void *p) {
      typedef ::genfit::HMatrixUnit current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::genfit::HMatrixUnit

namespace genfit {
//______________________________________________________________________________
void HMatrixV::Streamer(TBuffer &R__b)
{
   // Stream an object of class genfit::HMatrixV.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(genfit::HMatrixV::Class(),this);
   } else {
      R__b.WriteClassBuffer(genfit::HMatrixV::Class(),this);
   }
}

} // namespace genfit
namespace ROOT {
   // Wrappers around operator new
   static void *new_genfitcLcLHMatrixV(void *p) {
      return  p ? new(p) ::genfit::HMatrixV : new ::genfit::HMatrixV;
   }
   static void *newArray_genfitcLcLHMatrixV(Long_t nElements, void *p) {
      return p ? new(p) ::genfit::HMatrixV[nElements] : new ::genfit::HMatrixV[nElements];
   }
   // Wrapper around operator delete
   static void delete_genfitcLcLHMatrixV(void *p) {
      delete ((::genfit::HMatrixV*)p);
   }
   static void deleteArray_genfitcLcLHMatrixV(void *p) {
      delete [] ((::genfit::HMatrixV*)p);
   }
   static void destruct_genfitcLcLHMatrixV(void *p) {
      typedef ::genfit::HMatrixV current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::genfit::HMatrixV

namespace genfit {
//______________________________________________________________________________
void HMatrixUV::Streamer(TBuffer &R__b)
{
   // Stream an object of class genfit::HMatrixUV.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(genfit::HMatrixUV::Class(),this);
   } else {
      R__b.WriteClassBuffer(genfit::HMatrixUV::Class(),this);
   }
}

} // namespace genfit
namespace ROOT {
   // Wrappers around operator new
   static void *new_genfitcLcLHMatrixUV(void *p) {
      return  p ? new(p) ::genfit::HMatrixUV : new ::genfit::HMatrixUV;
   }
   static void *newArray_genfitcLcLHMatrixUV(Long_t nElements, void *p) {
      return p ? new(p) ::genfit::HMatrixUV[nElements] : new ::genfit::HMatrixUV[nElements];
   }
   // Wrapper around operator delete
   static void delete_genfitcLcLHMatrixUV(void *p) {
      delete ((::genfit::HMatrixUV*)p);
   }
   static void deleteArray_genfitcLcLHMatrixUV(void *p) {
      delete [] ((::genfit::HMatrixUV*)p);
   }
   static void destruct_genfitcLcLHMatrixUV(void *p) {
      typedef ::genfit::HMatrixUV current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::genfit::HMatrixUV

namespace ROOT {
   // Wrappers around operator new
   static void *new_genfitcLcLFullMeasurement(void *p) {
      return  p ? new(p) ::genfit::FullMeasurement : new ::genfit::FullMeasurement;
   }
   static void *newArray_genfitcLcLFullMeasurement(Long_t nElements, void *p) {
      return p ? new(p) ::genfit::FullMeasurement[nElements] : new ::genfit::FullMeasurement[nElements];
   }
   // Wrapper around operator delete
   static void delete_genfitcLcLFullMeasurement(void *p) {
      delete ((::genfit::FullMeasurement*)p);
   }
   static void deleteArray_genfitcLcLFullMeasurement(void *p) {
      delete [] ((::genfit::FullMeasurement*)p);
   }
   static void destruct_genfitcLcLFullMeasurement(void *p) {
      typedef ::genfit::FullMeasurement current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_genfitcLcLFullMeasurement(TBuffer &buf, void *obj) {
      ((::genfit::FullMeasurement*)obj)->::genfit::FullMeasurement::Streamer(buf);
   }
} // end of namespace ROOT for class ::genfit::FullMeasurement

namespace ROOT {
   // Wrappers around operator new
   static void *new_genfitcLcLPlanarMeasurement(void *p) {
      return  p ? new(p) ::genfit::PlanarMeasurement : new ::genfit::PlanarMeasurement;
   }
   static void *newArray_genfitcLcLPlanarMeasurement(Long_t nElements, void *p) {
      return p ? new(p) ::genfit::PlanarMeasurement[nElements] : new ::genfit::PlanarMeasurement[nElements];
   }
   // Wrapper around operator delete
   static void delete_genfitcLcLPlanarMeasurement(void *p) {
      delete ((::genfit::PlanarMeasurement*)p);
   }
   static void deleteArray_genfitcLcLPlanarMeasurement(void *p) {
      delete [] ((::genfit::PlanarMeasurement*)p);
   }
   static void destruct_genfitcLcLPlanarMeasurement(void *p) {
      typedef ::genfit::PlanarMeasurement current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_genfitcLcLPlanarMeasurement(TBuffer &buf, void *obj) {
      ((::genfit::PlanarMeasurement*)obj)->::genfit::PlanarMeasurement::Streamer(buf);
   }
} // end of namespace ROOT for class ::genfit::PlanarMeasurement

namespace ROOT {
   // Wrappers around operator new
   static void *new_genfitcLcLSpacepointMeasurement(void *p) {
      return  p ? new(p) ::genfit::SpacepointMeasurement : new ::genfit::SpacepointMeasurement;
   }
   static void *newArray_genfitcLcLSpacepointMeasurement(Long_t nElements, void *p) {
      return p ? new(p) ::genfit::SpacepointMeasurement[nElements] : new ::genfit::SpacepointMeasurement[nElements];
   }
   // Wrapper around operator delete
   static void delete_genfitcLcLSpacepointMeasurement(void *p) {
      delete ((::genfit::SpacepointMeasurement*)p);
   }
   static void deleteArray_genfitcLcLSpacepointMeasurement(void *p) {
      delete [] ((::genfit::SpacepointMeasurement*)p);
   }
   static void destruct_genfitcLcLSpacepointMeasurement(void *p) {
      typedef ::genfit::SpacepointMeasurement current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_genfitcLcLSpacepointMeasurement(TBuffer &buf, void *obj) {
      ((::genfit::SpacepointMeasurement*)obj)->::genfit::SpacepointMeasurement::Streamer(buf);
   }
} // end of namespace ROOT for class ::genfit::SpacepointMeasurement

namespace genfit {
//______________________________________________________________________________
void ProlateSpacepointMeasurement::Streamer(TBuffer &R__b)
{
   // Stream an object of class genfit::ProlateSpacepointMeasurement.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(genfit::ProlateSpacepointMeasurement::Class(),this);
   } else {
      R__b.WriteClassBuffer(genfit::ProlateSpacepointMeasurement::Class(),this);
   }
}

} // namespace genfit
namespace ROOT {
   // Wrappers around operator new
   static void *new_genfitcLcLProlateSpacepointMeasurement(void *p) {
      return  p ? new(p) ::genfit::ProlateSpacepointMeasurement : new ::genfit::ProlateSpacepointMeasurement;
   }
   static void *newArray_genfitcLcLProlateSpacepointMeasurement(Long_t nElements, void *p) {
      return p ? new(p) ::genfit::ProlateSpacepointMeasurement[nElements] : new ::genfit::ProlateSpacepointMeasurement[nElements];
   }
   // Wrapper around operator delete
   static void delete_genfitcLcLProlateSpacepointMeasurement(void *p) {
      delete ((::genfit::ProlateSpacepointMeasurement*)p);
   }
   static void deleteArray_genfitcLcLProlateSpacepointMeasurement(void *p) {
      delete [] ((::genfit::ProlateSpacepointMeasurement*)p);
   }
   static void destruct_genfitcLcLProlateSpacepointMeasurement(void *p) {
      typedef ::genfit::ProlateSpacepointMeasurement current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::genfit::ProlateSpacepointMeasurement

namespace genfit {
//______________________________________________________________________________
void WireMeasurement::Streamer(TBuffer &R__b)
{
   // Stream an object of class genfit::WireMeasurement.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(genfit::WireMeasurement::Class(),this);
   } else {
      R__b.WriteClassBuffer(genfit::WireMeasurement::Class(),this);
   }
}

} // namespace genfit
namespace ROOT {
   // Wrappers around operator new
   static void *new_genfitcLcLWireMeasurement(void *p) {
      return  p ? new(p) ::genfit::WireMeasurement : new ::genfit::WireMeasurement;
   }
   static void *newArray_genfitcLcLWireMeasurement(Long_t nElements, void *p) {
      return p ? new(p) ::genfit::WireMeasurement[nElements] : new ::genfit::WireMeasurement[nElements];
   }
   // Wrapper around operator delete
   static void delete_genfitcLcLWireMeasurement(void *p) {
      delete ((::genfit::WireMeasurement*)p);
   }
   static void deleteArray_genfitcLcLWireMeasurement(void *p) {
      delete [] ((::genfit::WireMeasurement*)p);
   }
   static void destruct_genfitcLcLWireMeasurement(void *p) {
      typedef ::genfit::WireMeasurement current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::genfit::WireMeasurement

namespace genfit {
//______________________________________________________________________________
void WireMeasurementNew::Streamer(TBuffer &R__b)
{
   // Stream an object of class genfit::WireMeasurementNew.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(genfit::WireMeasurementNew::Class(),this);
   } else {
      R__b.WriteClassBuffer(genfit::WireMeasurementNew::Class(),this);
   }
}

} // namespace genfit
namespace ROOT {
   // Wrappers around operator new
   static void *new_genfitcLcLWireMeasurementNew(void *p) {
      return  p ? new(p) ::genfit::WireMeasurementNew : new ::genfit::WireMeasurementNew;
   }
   static void *newArray_genfitcLcLWireMeasurementNew(Long_t nElements, void *p) {
      return p ? new(p) ::genfit::WireMeasurementNew[nElements] : new ::genfit::WireMeasurementNew[nElements];
   }
   // Wrapper around operator delete
   static void delete_genfitcLcLWireMeasurementNew(void *p) {
      delete ((::genfit::WireMeasurementNew*)p);
   }
   static void deleteArray_genfitcLcLWireMeasurementNew(void *p) {
      delete [] ((::genfit::WireMeasurementNew*)p);
   }
   static void destruct_genfitcLcLWireMeasurementNew(void *p) {
      typedef ::genfit::WireMeasurementNew current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::genfit::WireMeasurementNew

namespace genfit {
//______________________________________________________________________________
void WirePointMeasurement::Streamer(TBuffer &R__b)
{
   // Stream an object of class genfit::WirePointMeasurement.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(genfit::WirePointMeasurement::Class(),this);
   } else {
      R__b.WriteClassBuffer(genfit::WirePointMeasurement::Class(),this);
   }
}

} // namespace genfit
namespace ROOT {
   // Wrappers around operator new
   static void *new_genfitcLcLWirePointMeasurement(void *p) {
      return  p ? new(p) ::genfit::WirePointMeasurement : new ::genfit::WirePointMeasurement;
   }
   static void *newArray_genfitcLcLWirePointMeasurement(Long_t nElements, void *p) {
      return p ? new(p) ::genfit::WirePointMeasurement[nElements] : new ::genfit::WirePointMeasurement[nElements];
   }
   // Wrapper around operator delete
   static void delete_genfitcLcLWirePointMeasurement(void *p) {
      delete ((::genfit::WirePointMeasurement*)p);
   }
   static void deleteArray_genfitcLcLWirePointMeasurement(void *p) {
      delete [] ((::genfit::WirePointMeasurement*)p);
   }
   static void destruct_genfitcLcLWirePointMeasurement(void *p) {
      typedef ::genfit::WirePointMeasurement current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::genfit::WirePointMeasurement

namespace genfit {
//______________________________________________________________________________
void WireTrackCandHit::Streamer(TBuffer &R__b)
{
   // Stream an object of class genfit::WireTrackCandHit.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(genfit::WireTrackCandHit::Class(),this);
   } else {
      R__b.WriteClassBuffer(genfit::WireTrackCandHit::Class(),this);
   }
}

} // namespace genfit
namespace ROOT {
   // Wrappers around operator new
   static void *new_genfitcLcLWireTrackCandHit(void *p) {
      return  p ? new(p) ::genfit::WireTrackCandHit : new ::genfit::WireTrackCandHit;
   }
   static void *newArray_genfitcLcLWireTrackCandHit(Long_t nElements, void *p) {
      return p ? new(p) ::genfit::WireTrackCandHit[nElements] : new ::genfit::WireTrackCandHit[nElements];
   }
   // Wrapper around operator delete
   static void delete_genfitcLcLWireTrackCandHit(void *p) {
      delete ((::genfit::WireTrackCandHit*)p);
   }
   static void deleteArray_genfitcLcLWireTrackCandHit(void *p) {
      delete [] ((::genfit::WireTrackCandHit*)p);
   }
   static void destruct_genfitcLcLWireTrackCandHit(void *p) {
      typedef ::genfit::WireTrackCandHit current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::genfit::WireTrackCandHit

namespace {
  void TriggerDictionaryInitialization_libmeasurementsRootDict_Impl() {
    static const char* headers[] = {
"/tmp/source/GenFit2/measurements/include/HMatrixPhi.h",
"/tmp/source/GenFit2/measurements/include/HMatrixU.h",
"/tmp/source/GenFit2/measurements/include/HMatrixUnit.h",
"/tmp/source/GenFit2/measurements/include/HMatrixV.h",
"/tmp/source/GenFit2/measurements/include/HMatrixUV.h",
"/tmp/source/GenFit2/measurements/include/FullMeasurement.h",
"/tmp/source/GenFit2/measurements/include/PlanarMeasurement.h",
"/tmp/source/GenFit2/measurements/include/ProlateSpacepointMeasurement.h",
"/tmp/source/GenFit2/measurements/include/SpacepointMeasurement.h",
"/tmp/source/GenFit2/measurements/include/WireMeasurement.h",
"/tmp/source/GenFit2/measurements/include/WireMeasurementNew.h",
"/tmp/source/GenFit2/measurements/include/WirePointMeasurement.h",
"/tmp/source/GenFit2/measurements/include/WireTrackCandHit.h",
nullptr
    };
    static const char* includePaths[] = {
"/tmp/source/Utility/Object/include",
"/tmp/source/GenFit2/./trackReps/include",
"/tmp/source/GenFit2/./measurements/include",
"/tmp/source/GenFit2/./fitters/include",
"/tmp/source/GenFit2/./fields/include",
"/tmp/source/GenFit2/./core/include",
"/usr/include/eigen3",
"/opt/root/include",
"/opt/geant4/include/Geant4",
"/usr/include",
"/tmp/source/.",
"/tmp/source/GenFit2/core/include",
"/tmp/source/GenFit2/fields/include",
"/tmp/source/GenFit2/fitters/include",
"/tmp/source/GenFit2/measurements/include",
"/tmp/source/GenFit2/trackReps/include",
"/tmp/source/Utility/Object/include/Object",
"/tmp/source/Utility/UTIL/include",
"/tmp/source/Algorithms/CutFlowAnalysis/include",
"/tmp/source/Algorithms/Digitizer/include",
"/tmp/source/Algorithms/ExampleProcessor/include",
"/tmp/source/Algorithms/MCTruthAnalysis/include",
"/tmp/source/Algorithms/RecECAL/include",
"/tmp/source/Algorithms/Tracking/include",
"/tmp/source/Algorithms/Tracking/include/Algo",
"/tmp/source/GenFit2",
"/opt/root/include/",
"/tmp/source/cmake-build-debug-docker/GenFit2/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libmeasurementsRootDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
namespace genfit{class __attribute__((annotate("$clingAutoload$/tmp/source/GenFit2/measurements/include/HMatrixPhi.h")))  HMatrixPhi;}
namespace genfit{class __attribute__((annotate("$clingAutoload$/tmp/source/GenFit2/measurements/include/HMatrixU.h")))  HMatrixU;}
namespace genfit{class __attribute__((annotate("$clingAutoload$/tmp/source/GenFit2/measurements/include/HMatrixUnit.h")))  HMatrixUnit;}
namespace genfit{class __attribute__((annotate("$clingAutoload$/tmp/source/GenFit2/measurements/include/HMatrixV.h")))  HMatrixV;}
namespace genfit{class __attribute__((annotate("$clingAutoload$/tmp/source/GenFit2/measurements/include/HMatrixUV.h")))  HMatrixUV;}
namespace genfit{class __attribute__((annotate("$clingAutoload$/tmp/source/GenFit2/measurements/include/FullMeasurement.h")))  FullMeasurement;}
namespace genfit{class __attribute__((annotate("$clingAutoload$/tmp/source/GenFit2/measurements/include/PlanarMeasurement.h")))  PlanarMeasurement;}
namespace genfit{class __attribute__((annotate("$clingAutoload$SpacepointMeasurement.h")))  __attribute__((annotate("$clingAutoload$/tmp/source/GenFit2/measurements/include/ProlateSpacepointMeasurement.h")))  SpacepointMeasurement;}
namespace genfit{class __attribute__((annotate("$clingAutoload$/tmp/source/GenFit2/measurements/include/ProlateSpacepointMeasurement.h")))  ProlateSpacepointMeasurement;}
namespace genfit{class __attribute__((annotate("$clingAutoload$/tmp/source/GenFit2/measurements/include/WireMeasurement.h")))  WireMeasurement;}
namespace genfit{class __attribute__((annotate("$clingAutoload$/tmp/source/GenFit2/measurements/include/WireMeasurementNew.h")))  WireMeasurementNew;}
namespace genfit{class __attribute__((annotate("$clingAutoload$/tmp/source/GenFit2/measurements/include/WirePointMeasurement.h")))  WirePointMeasurement;}
namespace genfit{class __attribute__((annotate("$clingAutoload$/tmp/source/GenFit2/measurements/include/WireTrackCandHit.h")))  WireTrackCandHit;}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libmeasurementsRootDict dictionary payload"

#ifndef G4UI_USE_TCSH
  #define G4UI_USE_TCSH 1
#endif
#ifndef G4INTY_USE_QT
  #define G4INTY_USE_QT 1
#endif
#ifndef G4UI_USE_QT
  #define G4UI_USE_QT 1
#endif
#ifndef G4VIS_USE_OPENGLQT
  #define G4VIS_USE_OPENGLQT 1
#endif
#ifndef G4INTY_USE_XT
  #define G4INTY_USE_XT 1
#endif
#ifndef G4UI_USE_XM
  #define G4UI_USE_XM 1
#endif
#ifndef G4VIS_USE_OPENGLXM
  #define G4VIS_USE_OPENGLXM 1
#endif
#ifndef G4VIS_USE_OPENGLX
  #define G4VIS_USE_OPENGLX 1
#endif
#ifndef G4VIS_USE_OPENGL
  #define G4VIS_USE_OPENGL 1
#endif
#ifndef DEBUG
  #define DEBUG 1
#endif
#ifndef MEMCK
  #define MEMCK 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "/tmp/source/GenFit2/measurements/include/HMatrixPhi.h"
#include "/tmp/source/GenFit2/measurements/include/HMatrixU.h"
#include "/tmp/source/GenFit2/measurements/include/HMatrixUnit.h"
#include "/tmp/source/GenFit2/measurements/include/HMatrixV.h"
#include "/tmp/source/GenFit2/measurements/include/HMatrixUV.h"
#include "/tmp/source/GenFit2/measurements/include/FullMeasurement.h"
#include "/tmp/source/GenFit2/measurements/include/PlanarMeasurement.h"
#include "/tmp/source/GenFit2/measurements/include/ProlateSpacepointMeasurement.h"
#include "/tmp/source/GenFit2/measurements/include/SpacepointMeasurement.h"
#include "/tmp/source/GenFit2/measurements/include/WireMeasurement.h"
#include "/tmp/source/GenFit2/measurements/include/WireMeasurementNew.h"
#include "/tmp/source/GenFit2/measurements/include/WirePointMeasurement.h"
#include "/tmp/source/GenFit2/measurements/include/WireTrackCandHit.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"genfit::FullMeasurement", payloadCode, "@",
"genfit::HMatrixPhi", payloadCode, "@",
"genfit::HMatrixU", payloadCode, "@",
"genfit::HMatrixUV", payloadCode, "@",
"genfit::HMatrixUnit", payloadCode, "@",
"genfit::HMatrixV", payloadCode, "@",
"genfit::PlanarMeasurement", payloadCode, "@",
"genfit::ProlateSpacepointMeasurement", payloadCode, "@",
"genfit::SpacepointMeasurement", payloadCode, "@",
"genfit::WireMeasurement", payloadCode, "@",
"genfit::WireMeasurementNew", payloadCode, "@",
"genfit::WirePointMeasurement", payloadCode, "@",
"genfit::WireTrackCandHit", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libmeasurementsRootDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libmeasurementsRootDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libmeasurementsRootDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libmeasurementsRootDict() {
  TriggerDictionaryInitialization_libmeasurementsRootDict_Impl();
}
