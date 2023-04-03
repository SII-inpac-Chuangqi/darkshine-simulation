// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME coreRootDict
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

#include "TBuffer.h"
#include "TVirtualObject.h"
#include <vector>
#include "TSchemaHelper.h"


// Header files passed as explicit arguments
#include "/tmp/source/GenFit2/core/include/AbsFinitePlane.h"
#include "/tmp/source/GenFit2/core/include/AbsFitterInfo.h"
#include "/tmp/source/GenFit2/core/include/AbsHMatrix.h"
#include "/tmp/source/GenFit2/core/include/AbsTrackRep.h"
#include "/tmp/source/GenFit2/core/include/DetPlane.h"
#include "/tmp/source/GenFit2/core/include/FitStatus.h"
#include "/tmp/source/GenFit2/core/include/IO.h"
#include "/tmp/source/GenFit2/core/include/Material.h"
#include "/tmp/source/GenFit2/core/include/MeasuredStateOnPlane.h"
#include "/tmp/source/GenFit2/core/include/MeasurementFactory.h"
#include "/tmp/source/GenFit2/core/include/MeasurementOnPlane.h"
#include "/tmp/source/GenFit2/core/include/MeasurementProducer.h"
#include "/tmp/source/GenFit2/core/include/StateOnPlane.h"
#include "/tmp/source/GenFit2/core/include/AbsMeasurement.h"
#include "/tmp/source/GenFit2/core/include/ThinScatterer.h"
#include "/tmp/source/GenFit2/core/include/Track.h"
#include "/tmp/source/GenFit2/core/include/TrackCand.h"
#include "/tmp/source/GenFit2/core/include/TrackCandHit.h"
#include "/tmp/source/GenFit2/core/include/TrackPoint.h"

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
            instance("genfit", 0 /*version*/, "AbsFinitePlane.h", 35,
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
   static void delete_genfitcLcLAbsFinitePlane(void *p);
   static void deleteArray_genfitcLcLAbsFinitePlane(void *p);
   static void destruct_genfitcLcLAbsFinitePlane(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::genfit::AbsFinitePlane*)
   {
      ::genfit::AbsFinitePlane *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::genfit::AbsFinitePlane >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("genfit::AbsFinitePlane", ::genfit::AbsFinitePlane::Class_Version(), "AbsFinitePlane.h", 43,
                  typeid(::genfit::AbsFinitePlane), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::genfit::AbsFinitePlane::Dictionary, isa_proxy, 4,
                  sizeof(::genfit::AbsFinitePlane) );
      instance.SetDelete(&delete_genfitcLcLAbsFinitePlane);
      instance.SetDeleteArray(&deleteArray_genfitcLcLAbsFinitePlane);
      instance.SetDestructor(&destruct_genfitcLcLAbsFinitePlane);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::genfit::AbsFinitePlane*)
   {
      return GenerateInitInstanceLocal((::genfit::AbsFinitePlane*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::genfit::AbsFinitePlane*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_genfitcLcLDetPlane(void *p = nullptr);
   static void *newArray_genfitcLcLDetPlane(Long_t size, void *p);
   static void delete_genfitcLcLDetPlane(void *p);
   static void deleteArray_genfitcLcLDetPlane(void *p);
   static void destruct_genfitcLcLDetPlane(void *p);
   static void streamer_genfitcLcLDetPlane(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::genfit::DetPlane*)
   {
      ::genfit::DetPlane *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::genfit::DetPlane >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("genfit::DetPlane", ::genfit::DetPlane::Class_Version(), "DetPlane.h", 59,
                  typeid(::genfit::DetPlane), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::genfit::DetPlane::Dictionary, isa_proxy, 17,
                  sizeof(::genfit::DetPlane) );
      instance.SetNew(&new_genfitcLcLDetPlane);
      instance.SetNewArray(&newArray_genfitcLcLDetPlane);
      instance.SetDelete(&delete_genfitcLcLDetPlane);
      instance.SetDeleteArray(&deleteArray_genfitcLcLDetPlane);
      instance.SetDestructor(&destruct_genfitcLcLDetPlane);
      instance.SetStreamerFunc(&streamer_genfitcLcLDetPlane);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::genfit::DetPlane*)
   {
      return GenerateInitInstanceLocal((::genfit::DetPlane*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::genfit::DetPlane*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *genfitcLcLSharedPlanePtrCreator_Dictionary();
   static void genfitcLcLSharedPlanePtrCreator_TClassManip(TClass*);
   static void *new_genfitcLcLSharedPlanePtrCreator(void *p = nullptr);
   static void *newArray_genfitcLcLSharedPlanePtrCreator(Long_t size, void *p);
   static void delete_genfitcLcLSharedPlanePtrCreator(void *p);
   static void deleteArray_genfitcLcLSharedPlanePtrCreator(void *p);
   static void destruct_genfitcLcLSharedPlanePtrCreator(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::genfit::SharedPlanePtrCreator*)
   {
      ::genfit::SharedPlanePtrCreator *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::genfit::SharedPlanePtrCreator));
      static ::ROOT::TGenericClassInfo 
         instance("genfit::SharedPlanePtrCreator", "SharedPlanePtr.h", 45,
                  typeid(::genfit::SharedPlanePtrCreator), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &genfitcLcLSharedPlanePtrCreator_Dictionary, isa_proxy, 1,
                  sizeof(::genfit::SharedPlanePtrCreator) );
      instance.SetNew(&new_genfitcLcLSharedPlanePtrCreator);
      instance.SetNewArray(&newArray_genfitcLcLSharedPlanePtrCreator);
      instance.SetDelete(&delete_genfitcLcLSharedPlanePtrCreator);
      instance.SetDeleteArray(&deleteArray_genfitcLcLSharedPlanePtrCreator);
      instance.SetDestructor(&destruct_genfitcLcLSharedPlanePtrCreator);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::genfit::SharedPlanePtrCreator*)
   {
      return GenerateInitInstanceLocal((::genfit::SharedPlanePtrCreator*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::genfit::SharedPlanePtrCreator*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *genfitcLcLSharedPlanePtrCreator_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::genfit::SharedPlanePtrCreator*)nullptr)->GetClass();
      genfitcLcLSharedPlanePtrCreator_TClassManip(theClass);
   return theClass;
   }

   static void genfitcLcLSharedPlanePtrCreator_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void *new_genfitcLcLMaterial(void *p = nullptr);
   static void *newArray_genfitcLcLMaterial(Long_t size, void *p);
   static void delete_genfitcLcLMaterial(void *p);
   static void deleteArray_genfitcLcLMaterial(void *p);
   static void destruct_genfitcLcLMaterial(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::genfit::Material*)
   {
      ::genfit::Material *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::genfit::Material >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("genfit::Material", ::genfit::Material::Class_Version(), "Material.h", 8,
                  typeid(::genfit::Material), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::genfit::Material::Dictionary, isa_proxy, 4,
                  sizeof(::genfit::Material) );
      instance.SetNew(&new_genfitcLcLMaterial);
      instance.SetNewArray(&newArray_genfitcLcLMaterial);
      instance.SetDelete(&delete_genfitcLcLMaterial);
      instance.SetDeleteArray(&deleteArray_genfitcLcLMaterial);
      instance.SetDestructor(&destruct_genfitcLcLMaterial);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::genfit::Material*)
   {
      return GenerateInitInstanceLocal((::genfit::Material*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::genfit::Material*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_genfitcLcLAbsTrackRep(void *p);
   static void deleteArray_genfitcLcLAbsTrackRep(void *p);
   static void destruct_genfitcLcLAbsTrackRep(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::genfit::AbsTrackRep*)
   {
      ::genfit::AbsTrackRep *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::genfit::AbsTrackRep >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("genfit::AbsTrackRep", ::genfit::AbsTrackRep::Class_Version(), "AbsTrackRep.h", 66,
                  typeid(::genfit::AbsTrackRep), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::genfit::AbsTrackRep::Dictionary, isa_proxy, 4,
                  sizeof(::genfit::AbsTrackRep) );
      instance.SetDelete(&delete_genfitcLcLAbsTrackRep);
      instance.SetDeleteArray(&deleteArray_genfitcLcLAbsTrackRep);
      instance.SetDestructor(&destruct_genfitcLcLAbsTrackRep);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::genfit::AbsTrackRep*)
   {
      return GenerateInitInstanceLocal((::genfit::AbsTrackRep*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::genfit::AbsTrackRep*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_genfitcLcLStateOnPlane(void *p = nullptr);
   static void *newArray_genfitcLcLStateOnPlane(Long_t size, void *p);
   static void delete_genfitcLcLStateOnPlane(void *p);
   static void deleteArray_genfitcLcLStateOnPlane(void *p);
   static void destruct_genfitcLcLStateOnPlane(void *p);
   static void streamer_genfitcLcLStateOnPlane(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::genfit::StateOnPlane*)
   {
      ::genfit::StateOnPlane *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::genfit::StateOnPlane >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("genfit::StateOnPlane", ::genfit::StateOnPlane::Class_Version(), "StateOnPlane.h", 47,
                  typeid(::genfit::StateOnPlane), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::genfit::StateOnPlane::Dictionary, isa_proxy, 17,
                  sizeof(::genfit::StateOnPlane) );
      instance.SetNew(&new_genfitcLcLStateOnPlane);
      instance.SetNewArray(&newArray_genfitcLcLStateOnPlane);
      instance.SetDelete(&delete_genfitcLcLStateOnPlane);
      instance.SetDeleteArray(&deleteArray_genfitcLcLStateOnPlane);
      instance.SetDestructor(&destruct_genfitcLcLStateOnPlane);
      instance.SetStreamerFunc(&streamer_genfitcLcLStateOnPlane);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::genfit::StateOnPlane*)
   {
      return GenerateInitInstanceLocal((::genfit::StateOnPlane*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::genfit::StateOnPlane*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_genfitcLcLMeasuredStateOnPlane(void *p = nullptr);
   static void *newArray_genfitcLcLMeasuredStateOnPlane(Long_t size, void *p);
   static void delete_genfitcLcLMeasuredStateOnPlane(void *p);
   static void deleteArray_genfitcLcLMeasuredStateOnPlane(void *p);
   static void destruct_genfitcLcLMeasuredStateOnPlane(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::genfit::MeasuredStateOnPlane*)
   {
      ::genfit::MeasuredStateOnPlane *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::genfit::MeasuredStateOnPlane >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("genfit::MeasuredStateOnPlane", ::genfit::MeasuredStateOnPlane::Class_Version(), "MeasuredStateOnPlane.h", 39,
                  typeid(::genfit::MeasuredStateOnPlane), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::genfit::MeasuredStateOnPlane::Dictionary, isa_proxy, 4,
                  sizeof(::genfit::MeasuredStateOnPlane) );
      instance.SetNew(&new_genfitcLcLMeasuredStateOnPlane);
      instance.SetNewArray(&newArray_genfitcLcLMeasuredStateOnPlane);
      instance.SetDelete(&delete_genfitcLcLMeasuredStateOnPlane);
      instance.SetDeleteArray(&deleteArray_genfitcLcLMeasuredStateOnPlane);
      instance.SetDestructor(&destruct_genfitcLcLMeasuredStateOnPlane);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::genfit::MeasuredStateOnPlane*)
   {
      return GenerateInitInstanceLocal((::genfit::MeasuredStateOnPlane*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::genfit::MeasuredStateOnPlane*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_genfitcLcLAbsHMatrix(void *p);
   static void deleteArray_genfitcLcLAbsHMatrix(void *p);
   static void destruct_genfitcLcLAbsHMatrix(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::genfit::AbsHMatrix*)
   {
      ::genfit::AbsHMatrix *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::genfit::AbsHMatrix >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("genfit::AbsHMatrix", ::genfit::AbsHMatrix::Class_Version(), "AbsHMatrix.h", 37,
                  typeid(::genfit::AbsHMatrix), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::genfit::AbsHMatrix::Dictionary, isa_proxy, 4,
                  sizeof(::genfit::AbsHMatrix) );
      instance.SetDelete(&delete_genfitcLcLAbsHMatrix);
      instance.SetDeleteArray(&deleteArray_genfitcLcLAbsHMatrix);
      instance.SetDestructor(&destruct_genfitcLcLAbsHMatrix);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::genfit::AbsHMatrix*)
   {
      return GenerateInitInstanceLocal((::genfit::AbsHMatrix*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::genfit::AbsHMatrix*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_genfitcLcLMeasurementOnPlane(void *p = nullptr);
   static void *newArray_genfitcLcLMeasurementOnPlane(Long_t size, void *p);
   static void delete_genfitcLcLMeasurementOnPlane(void *p);
   static void deleteArray_genfitcLcLMeasurementOnPlane(void *p);
   static void destruct_genfitcLcLMeasurementOnPlane(void *p);
   static void streamer_genfitcLcLMeasurementOnPlane(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::genfit::MeasurementOnPlane*)
   {
      ::genfit::MeasurementOnPlane *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::genfit::MeasurementOnPlane >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("genfit::MeasurementOnPlane", ::genfit::MeasurementOnPlane::Class_Version(), "MeasurementOnPlane.h", 46,
                  typeid(::genfit::MeasurementOnPlane), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::genfit::MeasurementOnPlane::Dictionary, isa_proxy, 17,
                  sizeof(::genfit::MeasurementOnPlane) );
      instance.SetNew(&new_genfitcLcLMeasurementOnPlane);
      instance.SetNewArray(&newArray_genfitcLcLMeasurementOnPlane);
      instance.SetDelete(&delete_genfitcLcLMeasurementOnPlane);
      instance.SetDeleteArray(&deleteArray_genfitcLcLMeasurementOnPlane);
      instance.SetDestructor(&destruct_genfitcLcLMeasurementOnPlane);
      instance.SetStreamerFunc(&streamer_genfitcLcLMeasurementOnPlane);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::genfit::MeasurementOnPlane*)
   {
      return GenerateInitInstanceLocal((::genfit::MeasurementOnPlane*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::genfit::MeasurementOnPlane*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *genfitcLcLPruneFlags_Dictionary();
   static void genfitcLcLPruneFlags_TClassManip(TClass*);
   static void *new_genfitcLcLPruneFlags(void *p = nullptr);
   static void *newArray_genfitcLcLPruneFlags(Long_t size, void *p);
   static void delete_genfitcLcLPruneFlags(void *p);
   static void deleteArray_genfitcLcLPruneFlags(void *p);
   static void destruct_genfitcLcLPruneFlags(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::genfit::PruneFlags*)
   {
      ::genfit::PruneFlags *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::genfit::PruneFlags));
      static ::ROOT::TGenericClassInfo 
         instance("genfit::PruneFlags", "FitStatus.h", 47,
                  typeid(::genfit::PruneFlags), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &genfitcLcLPruneFlags_Dictionary, isa_proxy, 4,
                  sizeof(::genfit::PruneFlags) );
      instance.SetNew(&new_genfitcLcLPruneFlags);
      instance.SetNewArray(&newArray_genfitcLcLPruneFlags);
      instance.SetDelete(&delete_genfitcLcLPruneFlags);
      instance.SetDeleteArray(&deleteArray_genfitcLcLPruneFlags);
      instance.SetDestructor(&destruct_genfitcLcLPruneFlags);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::genfit::PruneFlags*)
   {
      return GenerateInitInstanceLocal((::genfit::PruneFlags*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::genfit::PruneFlags*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *genfitcLcLPruneFlags_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::genfit::PruneFlags*)nullptr)->GetClass();
      genfitcLcLPruneFlags_TClassManip(theClass);
   return theClass;
   }

   static void genfitcLcLPruneFlags_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void *new_genfitcLcLFitStatus(void *p = nullptr);
   static void *newArray_genfitcLcLFitStatus(Long_t size, void *p);
   static void delete_genfitcLcLFitStatus(void *p);
   static void deleteArray_genfitcLcLFitStatus(void *p);
   static void destruct_genfitcLcLFitStatus(void *p);

   // Schema evolution read functions
   static void read_genfitcLcLFitStatus_0( char* target, TVirtualObject *oldObj )
   {
      //--- Automatically generated variables ---
#if 0
      static Int_t id_trackIsPruned_ = oldObj->GetId("trackIsPruned_");
#endif
      struct genfitcLcLFitStatus_Onfile {
         bool &trackIsPruned_;
         genfitcLcLFitStatus_Onfile(bool &onfile_trackIsPruned_ ): trackIsPruned_(onfile_trackIsPruned_) {}
      };
      static Long_t offset_Onfile_genfitcLcLFitStatus_trackIsPruned_ = oldObj->GetClass()->GetDataMemberOffset("trackIsPruned_");
      char *onfile_add = (char*)oldObj->GetObject();
      genfitcLcLFitStatus_Onfile onfile(
         *(bool*)(onfile_add+offset_Onfile_genfitcLcLFitStatus_trackIsPruned_) );

      static TClassRef cls("genfit::FitStatus");
      static Long_t offset_pruneFlags_ = cls->GetDataMemberOffset("pruneFlags_");
      genfit::PruneFlags& pruneFlags_ = *(genfit::PruneFlags*)(target+offset_pruneFlags_);
      genfit::FitStatus* newObj = (genfit::FitStatus*)target;
      // Supress warning message.
      (void)oldObj;

      (void)newObj;

      //--- User's code ---
      pruneFlags_.setFlags(); 
   }
   static void read_genfitcLcLFitStatus_1( char* target, TVirtualObject *oldObj )
   {
      //--- Automatically generated variables ---
      static TClassRef cls("genfit::FitStatus");
      static Long_t offset_pruneFlags_ = cls->GetDataMemberOffset("pruneFlags_");
      genfit::PruneFlags& pruneFlags_ = *(genfit::PruneFlags*)(target+offset_pruneFlags_);
      genfit::FitStatus* newObj = (genfit::FitStatus*)target;
      // Supress warning message.
      (void)oldObj;

      (void)newObj;

      //--- User's code ---
      pruneFlags_.setFlags(); 
   }

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::genfit::FitStatus*)
   {
      ::genfit::FitStatus *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::genfit::FitStatus >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("genfit::FitStatus", ::genfit::FitStatus::Class_Version(), "FitStatus.h", 80,
                  typeid(::genfit::FitStatus), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::genfit::FitStatus::Dictionary, isa_proxy, 4,
                  sizeof(::genfit::FitStatus) );
      instance.SetNew(&new_genfitcLcLFitStatus);
      instance.SetNewArray(&newArray_genfitcLcLFitStatus);
      instance.SetDelete(&delete_genfitcLcLFitStatus);
      instance.SetDeleteArray(&deleteArray_genfitcLcLFitStatus);
      instance.SetDestructor(&destruct_genfitcLcLFitStatus);

      ::ROOT::Internal::TSchemaHelper* rule;

      // the io read rules
      std::vector<::ROOT::Internal::TSchemaHelper> readrules(2);
      rule = &readrules[0];
      rule->fSourceClass = "genfit::FitStatus";
      rule->fTarget      = "pruneFlags_";
      rule->fSource      = "bool trackIsPruned_;";
      rule->fFunctionPtr = (void *)TFunc2void( read_genfitcLcLFitStatus_0);
      rule->fCode        = " pruneFlags_.setFlags(); ";
      rule->fVersion     = "[1]";
      rule = &readrules[1];
      rule->fSourceClass = "genfit::FitStatus";
      rule->fTarget      = "pruneFlags_";
      rule->fSource      = "";
      rule->fFunctionPtr = (void *)TFunc2void( read_genfitcLcLFitStatus_1);
      rule->fCode        = " pruneFlags_.setFlags(); ";
      rule->fVersion     = "[2]";
      instance.SetReadRules( readrules );
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::genfit::FitStatus*)
   {
      return GenerateInitInstanceLocal((::genfit::FitStatus*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::genfit::FitStatus*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_genfitcLcLAbsFitterInfo(void *p);
   static void deleteArray_genfitcLcLAbsFitterInfo(void *p);
   static void destruct_genfitcLcLAbsFitterInfo(void *p);
   static void streamer_genfitcLcLAbsFitterInfo(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::genfit::AbsFitterInfo*)
   {
      ::genfit::AbsFitterInfo *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::genfit::AbsFitterInfo >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("genfit::AbsFitterInfo", ::genfit::AbsFitterInfo::Class_Version(), "AbsFitterInfo.h", 42,
                  typeid(::genfit::AbsFitterInfo), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::genfit::AbsFitterInfo::Dictionary, isa_proxy, 17,
                  sizeof(::genfit::AbsFitterInfo) );
      instance.SetDelete(&delete_genfitcLcLAbsFitterInfo);
      instance.SetDeleteArray(&deleteArray_genfitcLcLAbsFitterInfo);
      instance.SetDestructor(&destruct_genfitcLcLAbsFitterInfo);
      instance.SetStreamerFunc(&streamer_genfitcLcLAbsFitterInfo);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::genfit::AbsFitterInfo*)
   {
      return GenerateInitInstanceLocal((::genfit::AbsFitterInfo*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::genfit::AbsFitterInfo*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_genfitcLcLTrackCandHit(void *p = nullptr);
   static void *newArray_genfitcLcLTrackCandHit(Long_t size, void *p);
   static void delete_genfitcLcLTrackCandHit(void *p);
   static void deleteArray_genfitcLcLTrackCandHit(void *p);
   static void destruct_genfitcLcLTrackCandHit(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::genfit::TrackCandHit*)
   {
      ::genfit::TrackCandHit *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::genfit::TrackCandHit >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("genfit::TrackCandHit", ::genfit::TrackCandHit::Class_Version(), "TrackCandHit.h", 34,
                  typeid(::genfit::TrackCandHit), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::genfit::TrackCandHit::Dictionary, isa_proxy, 4,
                  sizeof(::genfit::TrackCandHit) );
      instance.SetNew(&new_genfitcLcLTrackCandHit);
      instance.SetNewArray(&newArray_genfitcLcLTrackCandHit);
      instance.SetDelete(&delete_genfitcLcLTrackCandHit);
      instance.SetDeleteArray(&deleteArray_genfitcLcLTrackCandHit);
      instance.SetDestructor(&destruct_genfitcLcLTrackCandHit);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::genfit::TrackCandHit*)
   {
      return GenerateInitInstanceLocal((::genfit::TrackCandHit*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::genfit::TrackCandHit*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_genfitcLcLTrackCand(void *p = nullptr);
   static void *newArray_genfitcLcLTrackCand(Long_t size, void *p);
   static void delete_genfitcLcLTrackCand(void *p);
   static void deleteArray_genfitcLcLTrackCand(void *p);
   static void destruct_genfitcLcLTrackCand(void *p);

   // Schema evolution read functions
   static void read_genfitcLcLTrackCand_0( char* target, TVirtualObject *oldObj )
   {
      //--- Automatically generated variables ---
      static TClassRef cls("genfit::TrackCand");
      static Long_t offset_time_ = cls->GetDataMemberOffset("time_");
      double& time_ = *(double*)(target+offset_time_);
      genfit::TrackCand* newObj = (genfit::TrackCand*)target;
      // Supress warning message.
      (void)oldObj;

      (void)newObj;

      //--- User's code ---
      time_ = 0; 
   }

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::genfit::TrackCand*)
   {
      ::genfit::TrackCand *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::genfit::TrackCand >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("genfit::TrackCand", ::genfit::TrackCand::Class_Version(), "TrackCand.h", 69,
                  typeid(::genfit::TrackCand), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::genfit::TrackCand::Dictionary, isa_proxy, 4,
                  sizeof(::genfit::TrackCand) );
      instance.SetNew(&new_genfitcLcLTrackCand);
      instance.SetNewArray(&newArray_genfitcLcLTrackCand);
      instance.SetDelete(&delete_genfitcLcLTrackCand);
      instance.SetDeleteArray(&deleteArray_genfitcLcLTrackCand);
      instance.SetDestructor(&destruct_genfitcLcLTrackCand);

      ::ROOT::Internal::TSchemaHelper* rule;

      // the io read rules
      std::vector<::ROOT::Internal::TSchemaHelper> readrules(1);
      rule = &readrules[0];
      rule->fSourceClass = "genfit::TrackCand";
      rule->fTarget      = "time_";
      rule->fSource      = "";
      rule->fFunctionPtr = (void *)TFunc2void( read_genfitcLcLTrackCand_0);
      rule->fCode        = " time_ = 0; ";
      rule->fVersion     = "[1]";
      instance.SetReadRules( readrules );
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::genfit::TrackCand*)
   {
      return GenerateInitInstanceLocal((::genfit::TrackCand*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::genfit::TrackCand*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void delete_genfitcLcLAbsMeasurement(void *p);
   static void deleteArray_genfitcLcLAbsMeasurement(void *p);
   static void destruct_genfitcLcLAbsMeasurement(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::genfit::AbsMeasurement*)
   {
      ::genfit::AbsMeasurement *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::genfit::AbsMeasurement >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("genfit::AbsMeasurement", ::genfit::AbsMeasurement::Class_Version(), "AbsMeasurement.h", 42,
                  typeid(::genfit::AbsMeasurement), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::genfit::AbsMeasurement::Dictionary, isa_proxy, 4,
                  sizeof(::genfit::AbsMeasurement) );
      instance.SetDelete(&delete_genfitcLcLAbsMeasurement);
      instance.SetDeleteArray(&deleteArray_genfitcLcLAbsMeasurement);
      instance.SetDestructor(&destruct_genfitcLcLAbsMeasurement);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::genfit::AbsMeasurement*)
   {
      return GenerateInitInstanceLocal((::genfit::AbsMeasurement*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::genfit::AbsMeasurement*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_genfitcLcLThinScatterer(void *p = nullptr);
   static void *newArray_genfitcLcLThinScatterer(Long_t size, void *p);
   static void delete_genfitcLcLThinScatterer(void *p);
   static void deleteArray_genfitcLcLThinScatterer(void *p);
   static void destruct_genfitcLcLThinScatterer(void *p);
   static void streamer_genfitcLcLThinScatterer(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::genfit::ThinScatterer*)
   {
      ::genfit::ThinScatterer *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::genfit::ThinScatterer >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("genfit::ThinScatterer", ::genfit::ThinScatterer::Class_Version(), "ThinScatterer.h", 38,
                  typeid(::genfit::ThinScatterer), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::genfit::ThinScatterer::Dictionary, isa_proxy, 17,
                  sizeof(::genfit::ThinScatterer) );
      instance.SetNew(&new_genfitcLcLThinScatterer);
      instance.SetNewArray(&newArray_genfitcLcLThinScatterer);
      instance.SetDelete(&delete_genfitcLcLThinScatterer);
      instance.SetDeleteArray(&deleteArray_genfitcLcLThinScatterer);
      instance.SetDestructor(&destruct_genfitcLcLThinScatterer);
      instance.SetStreamerFunc(&streamer_genfitcLcLThinScatterer);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::genfit::ThinScatterer*)
   {
      return GenerateInitInstanceLocal((::genfit::ThinScatterer*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::genfit::ThinScatterer*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_genfitcLcLTrackPoint(void *p = nullptr);
   static void *newArray_genfitcLcLTrackPoint(Long_t size, void *p);
   static void delete_genfitcLcLTrackPoint(void *p);
   static void deleteArray_genfitcLcLTrackPoint(void *p);
   static void destruct_genfitcLcLTrackPoint(void *p);
   static void streamer_genfitcLcLTrackPoint(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::genfit::TrackPoint*)
   {
      ::genfit::TrackPoint *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::genfit::TrackPoint >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("genfit::TrackPoint", ::genfit::TrackPoint::Class_Version(), "TrackPoint.h", 46,
                  typeid(::genfit::TrackPoint), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::genfit::TrackPoint::Dictionary, isa_proxy, 17,
                  sizeof(::genfit::TrackPoint) );
      instance.SetNew(&new_genfitcLcLTrackPoint);
      instance.SetNewArray(&newArray_genfitcLcLTrackPoint);
      instance.SetDelete(&delete_genfitcLcLTrackPoint);
      instance.SetDeleteArray(&deleteArray_genfitcLcLTrackPoint);
      instance.SetDestructor(&destruct_genfitcLcLTrackPoint);
      instance.SetStreamerFunc(&streamer_genfitcLcLTrackPoint);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::genfit::TrackPoint*)
   {
      return GenerateInitInstanceLocal((::genfit::TrackPoint*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::genfit::TrackPoint*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_genfitcLcLTrack(void *p = nullptr);
   static void *newArray_genfitcLcLTrack(Long_t size, void *p);
   static void delete_genfitcLcLTrack(void *p);
   static void deleteArray_genfitcLcLTrack(void *p);
   static void destruct_genfitcLcLTrack(void *p);
   static void streamer_genfitcLcLTrack(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::genfit::Track*)
   {
      ::genfit::Track *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::genfit::Track >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("genfit::Track", ::genfit::Track::Class_Version(), "Track.h", 71,
                  typeid(::genfit::Track), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::genfit::Track::Dictionary, isa_proxy, 17,
                  sizeof(::genfit::Track) );
      instance.SetNew(&new_genfitcLcLTrack);
      instance.SetNewArray(&newArray_genfitcLcLTrack);
      instance.SetDelete(&delete_genfitcLcLTrack);
      instance.SetDeleteArray(&deleteArray_genfitcLcLTrack);
      instance.SetDestructor(&destruct_genfitcLcLTrack);
      instance.SetStreamerFunc(&streamer_genfitcLcLTrack);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::genfit::Track*)
   {
      return GenerateInitInstanceLocal((::genfit::Track*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::genfit::Track*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace genfit {
//______________________________________________________________________________
atomic_TClass_ptr AbsFinitePlane::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *AbsFinitePlane::Class_Name()
{
   return "genfit::AbsFinitePlane";
}

//______________________________________________________________________________
const char *AbsFinitePlane::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::AbsFinitePlane*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int AbsFinitePlane::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::AbsFinitePlane*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *AbsFinitePlane::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::AbsFinitePlane*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *AbsFinitePlane::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::AbsFinitePlane*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace genfit
namespace genfit {
//______________________________________________________________________________
atomic_TClass_ptr DetPlane::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *DetPlane::Class_Name()
{
   return "genfit::DetPlane";
}

//______________________________________________________________________________
const char *DetPlane::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::DetPlane*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int DetPlane::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::DetPlane*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *DetPlane::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::DetPlane*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *DetPlane::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::DetPlane*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace genfit
namespace genfit {
//______________________________________________________________________________
atomic_TClass_ptr Material::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *Material::Class_Name()
{
   return "genfit::Material";
}

//______________________________________________________________________________
const char *Material::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::Material*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int Material::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::Material*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Material::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::Material*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Material::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::Material*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace genfit
namespace genfit {
//______________________________________________________________________________
atomic_TClass_ptr AbsTrackRep::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *AbsTrackRep::Class_Name()
{
   return "genfit::AbsTrackRep";
}

//______________________________________________________________________________
const char *AbsTrackRep::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::AbsTrackRep*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int AbsTrackRep::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::AbsTrackRep*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *AbsTrackRep::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::AbsTrackRep*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *AbsTrackRep::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::AbsTrackRep*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace genfit
namespace genfit {
//______________________________________________________________________________
atomic_TClass_ptr StateOnPlane::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *StateOnPlane::Class_Name()
{
   return "genfit::StateOnPlane";
}

//______________________________________________________________________________
const char *StateOnPlane::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::StateOnPlane*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int StateOnPlane::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::StateOnPlane*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *StateOnPlane::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::StateOnPlane*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *StateOnPlane::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::StateOnPlane*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace genfit
namespace genfit {
//______________________________________________________________________________
atomic_TClass_ptr MeasuredStateOnPlane::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *MeasuredStateOnPlane::Class_Name()
{
   return "genfit::MeasuredStateOnPlane";
}

//______________________________________________________________________________
const char *MeasuredStateOnPlane::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::MeasuredStateOnPlane*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int MeasuredStateOnPlane::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::MeasuredStateOnPlane*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *MeasuredStateOnPlane::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::MeasuredStateOnPlane*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *MeasuredStateOnPlane::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::MeasuredStateOnPlane*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace genfit
namespace genfit {
//______________________________________________________________________________
atomic_TClass_ptr AbsHMatrix::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *AbsHMatrix::Class_Name()
{
   return "genfit::AbsHMatrix";
}

//______________________________________________________________________________
const char *AbsHMatrix::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::AbsHMatrix*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int AbsHMatrix::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::AbsHMatrix*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *AbsHMatrix::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::AbsHMatrix*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *AbsHMatrix::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::AbsHMatrix*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace genfit
namespace genfit {
//______________________________________________________________________________
atomic_TClass_ptr MeasurementOnPlane::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *MeasurementOnPlane::Class_Name()
{
   return "genfit::MeasurementOnPlane";
}

//______________________________________________________________________________
const char *MeasurementOnPlane::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::MeasurementOnPlane*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int MeasurementOnPlane::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::MeasurementOnPlane*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *MeasurementOnPlane::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::MeasurementOnPlane*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *MeasurementOnPlane::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::MeasurementOnPlane*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace genfit
namespace genfit {
//______________________________________________________________________________
atomic_TClass_ptr FitStatus::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *FitStatus::Class_Name()
{
   return "genfit::FitStatus";
}

//______________________________________________________________________________
const char *FitStatus::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::FitStatus*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int FitStatus::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::FitStatus*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *FitStatus::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::FitStatus*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *FitStatus::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::FitStatus*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace genfit
namespace genfit {
//______________________________________________________________________________
atomic_TClass_ptr AbsFitterInfo::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *AbsFitterInfo::Class_Name()
{
   return "genfit::AbsFitterInfo";
}

//______________________________________________________________________________
const char *AbsFitterInfo::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::AbsFitterInfo*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int AbsFitterInfo::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::AbsFitterInfo*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *AbsFitterInfo::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::AbsFitterInfo*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *AbsFitterInfo::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::AbsFitterInfo*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace genfit
namespace genfit {
//______________________________________________________________________________
atomic_TClass_ptr TrackCandHit::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *TrackCandHit::Class_Name()
{
   return "genfit::TrackCandHit";
}

//______________________________________________________________________________
const char *TrackCandHit::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::TrackCandHit*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int TrackCandHit::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::TrackCandHit*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TrackCandHit::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::TrackCandHit*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TrackCandHit::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::TrackCandHit*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace genfit
namespace genfit {
//______________________________________________________________________________
atomic_TClass_ptr TrackCand::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *TrackCand::Class_Name()
{
   return "genfit::TrackCand";
}

//______________________________________________________________________________
const char *TrackCand::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::TrackCand*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int TrackCand::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::TrackCand*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TrackCand::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::TrackCand*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TrackCand::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::TrackCand*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace genfit
namespace genfit {
//______________________________________________________________________________
atomic_TClass_ptr AbsMeasurement::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *AbsMeasurement::Class_Name()
{
   return "genfit::AbsMeasurement";
}

//______________________________________________________________________________
const char *AbsMeasurement::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::AbsMeasurement*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int AbsMeasurement::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::AbsMeasurement*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *AbsMeasurement::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::AbsMeasurement*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *AbsMeasurement::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::AbsMeasurement*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace genfit
namespace genfit {
//______________________________________________________________________________
atomic_TClass_ptr ThinScatterer::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *ThinScatterer::Class_Name()
{
   return "genfit::ThinScatterer";
}

//______________________________________________________________________________
const char *ThinScatterer::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::ThinScatterer*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int ThinScatterer::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::ThinScatterer*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ThinScatterer::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::ThinScatterer*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ThinScatterer::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::ThinScatterer*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace genfit
namespace genfit {
//______________________________________________________________________________
atomic_TClass_ptr TrackPoint::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *TrackPoint::Class_Name()
{
   return "genfit::TrackPoint";
}

//______________________________________________________________________________
const char *TrackPoint::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::TrackPoint*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int TrackPoint::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::TrackPoint*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TrackPoint::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::TrackPoint*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TrackPoint::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::TrackPoint*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace genfit
namespace genfit {
//______________________________________________________________________________
atomic_TClass_ptr Track::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *Track::Class_Name()
{
   return "genfit::Track";
}

//______________________________________________________________________________
const char *Track::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::Track*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int Track::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::Track*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Track::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::Track*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Track::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::Track*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace genfit
namespace genfit {
//______________________________________________________________________________
void AbsFinitePlane::Streamer(TBuffer &R__b)
{
   // Stream an object of class genfit::AbsFinitePlane.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(genfit::AbsFinitePlane::Class(),this);
   } else {
      R__b.WriteClassBuffer(genfit::AbsFinitePlane::Class(),this);
   }
}

} // namespace genfit
namespace ROOT {
   // Wrapper around operator delete
   static void delete_genfitcLcLAbsFinitePlane(void *p) {
      delete ((::genfit::AbsFinitePlane*)p);
   }
   static void deleteArray_genfitcLcLAbsFinitePlane(void *p) {
      delete [] ((::genfit::AbsFinitePlane*)p);
   }
   static void destruct_genfitcLcLAbsFinitePlane(void *p) {
      typedef ::genfit::AbsFinitePlane current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::genfit::AbsFinitePlane

namespace ROOT {
   // Wrappers around operator new
   static void *new_genfitcLcLDetPlane(void *p) {
      return  p ? new(p) ::genfit::DetPlane : new ::genfit::DetPlane;
   }
   static void *newArray_genfitcLcLDetPlane(Long_t nElements, void *p) {
      return p ? new(p) ::genfit::DetPlane[nElements] : new ::genfit::DetPlane[nElements];
   }
   // Wrapper around operator delete
   static void delete_genfitcLcLDetPlane(void *p) {
      delete ((::genfit::DetPlane*)p);
   }
   static void deleteArray_genfitcLcLDetPlane(void *p) {
      delete [] ((::genfit::DetPlane*)p);
   }
   static void destruct_genfitcLcLDetPlane(void *p) {
      typedef ::genfit::DetPlane current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_genfitcLcLDetPlane(TBuffer &buf, void *obj) {
      ((::genfit::DetPlane*)obj)->::genfit::DetPlane::Streamer(buf);
   }
} // end of namespace ROOT for class ::genfit::DetPlane

namespace ROOT {
   // Wrappers around operator new
   static void *new_genfitcLcLSharedPlanePtrCreator(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::genfit::SharedPlanePtrCreator : new ::genfit::SharedPlanePtrCreator;
   }
   static void *newArray_genfitcLcLSharedPlanePtrCreator(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::genfit::SharedPlanePtrCreator[nElements] : new ::genfit::SharedPlanePtrCreator[nElements];
   }
   // Wrapper around operator delete
   static void delete_genfitcLcLSharedPlanePtrCreator(void *p) {
      delete ((::genfit::SharedPlanePtrCreator*)p);
   }
   static void deleteArray_genfitcLcLSharedPlanePtrCreator(void *p) {
      delete [] ((::genfit::SharedPlanePtrCreator*)p);
   }
   static void destruct_genfitcLcLSharedPlanePtrCreator(void *p) {
      typedef ::genfit::SharedPlanePtrCreator current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::genfit::SharedPlanePtrCreator

namespace genfit {
//______________________________________________________________________________
void Material::Streamer(TBuffer &R__b)
{
   // Stream an object of class genfit::Material.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(genfit::Material::Class(),this);
   } else {
      R__b.WriteClassBuffer(genfit::Material::Class(),this);
   }
}

} // namespace genfit
namespace ROOT {
   // Wrappers around operator new
   static void *new_genfitcLcLMaterial(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::genfit::Material : new ::genfit::Material;
   }
   static void *newArray_genfitcLcLMaterial(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::genfit::Material[nElements] : new ::genfit::Material[nElements];
   }
   // Wrapper around operator delete
   static void delete_genfitcLcLMaterial(void *p) {
      delete ((::genfit::Material*)p);
   }
   static void deleteArray_genfitcLcLMaterial(void *p) {
      delete [] ((::genfit::Material*)p);
   }
   static void destruct_genfitcLcLMaterial(void *p) {
      typedef ::genfit::Material current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::genfit::Material

namespace genfit {
//______________________________________________________________________________
void AbsTrackRep::Streamer(TBuffer &R__b)
{
   // Stream an object of class genfit::AbsTrackRep.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(genfit::AbsTrackRep::Class(),this);
   } else {
      R__b.WriteClassBuffer(genfit::AbsTrackRep::Class(),this);
   }
}

} // namespace genfit
namespace ROOT {
   // Wrapper around operator delete
   static void delete_genfitcLcLAbsTrackRep(void *p) {
      delete ((::genfit::AbsTrackRep*)p);
   }
   static void deleteArray_genfitcLcLAbsTrackRep(void *p) {
      delete [] ((::genfit::AbsTrackRep*)p);
   }
   static void destruct_genfitcLcLAbsTrackRep(void *p) {
      typedef ::genfit::AbsTrackRep current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::genfit::AbsTrackRep

namespace ROOT {
   // Wrappers around operator new
   static void *new_genfitcLcLStateOnPlane(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::genfit::StateOnPlane : new ::genfit::StateOnPlane;
   }
   static void *newArray_genfitcLcLStateOnPlane(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::genfit::StateOnPlane[nElements] : new ::genfit::StateOnPlane[nElements];
   }
   // Wrapper around operator delete
   static void delete_genfitcLcLStateOnPlane(void *p) {
      delete ((::genfit::StateOnPlane*)p);
   }
   static void deleteArray_genfitcLcLStateOnPlane(void *p) {
      delete [] ((::genfit::StateOnPlane*)p);
   }
   static void destruct_genfitcLcLStateOnPlane(void *p) {
      typedef ::genfit::StateOnPlane current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_genfitcLcLStateOnPlane(TBuffer &buf, void *obj) {
      ((::genfit::StateOnPlane*)obj)->::genfit::StateOnPlane::Streamer(buf);
   }
} // end of namespace ROOT for class ::genfit::StateOnPlane

namespace genfit {
//______________________________________________________________________________
void MeasuredStateOnPlane::Streamer(TBuffer &R__b)
{
   // Stream an object of class genfit::MeasuredStateOnPlane.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(genfit::MeasuredStateOnPlane::Class(),this);
   } else {
      R__b.WriteClassBuffer(genfit::MeasuredStateOnPlane::Class(),this);
   }
}

} // namespace genfit
namespace ROOT {
   // Wrappers around operator new
   static void *new_genfitcLcLMeasuredStateOnPlane(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::genfit::MeasuredStateOnPlane : new ::genfit::MeasuredStateOnPlane;
   }
   static void *newArray_genfitcLcLMeasuredStateOnPlane(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::genfit::MeasuredStateOnPlane[nElements] : new ::genfit::MeasuredStateOnPlane[nElements];
   }
   // Wrapper around operator delete
   static void delete_genfitcLcLMeasuredStateOnPlane(void *p) {
      delete ((::genfit::MeasuredStateOnPlane*)p);
   }
   static void deleteArray_genfitcLcLMeasuredStateOnPlane(void *p) {
      delete [] ((::genfit::MeasuredStateOnPlane*)p);
   }
   static void destruct_genfitcLcLMeasuredStateOnPlane(void *p) {
      typedef ::genfit::MeasuredStateOnPlane current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::genfit::MeasuredStateOnPlane

namespace genfit {
//______________________________________________________________________________
void AbsHMatrix::Streamer(TBuffer &R__b)
{
   // Stream an object of class genfit::AbsHMatrix.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(genfit::AbsHMatrix::Class(),this);
   } else {
      R__b.WriteClassBuffer(genfit::AbsHMatrix::Class(),this);
   }
}

} // namespace genfit
namespace ROOT {
   // Wrapper around operator delete
   static void delete_genfitcLcLAbsHMatrix(void *p) {
      delete ((::genfit::AbsHMatrix*)p);
   }
   static void deleteArray_genfitcLcLAbsHMatrix(void *p) {
      delete [] ((::genfit::AbsHMatrix*)p);
   }
   static void destruct_genfitcLcLAbsHMatrix(void *p) {
      typedef ::genfit::AbsHMatrix current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::genfit::AbsHMatrix

namespace ROOT {
   // Wrappers around operator new
   static void *new_genfitcLcLMeasurementOnPlane(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::genfit::MeasurementOnPlane : new ::genfit::MeasurementOnPlane;
   }
   static void *newArray_genfitcLcLMeasurementOnPlane(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::genfit::MeasurementOnPlane[nElements] : new ::genfit::MeasurementOnPlane[nElements];
   }
   // Wrapper around operator delete
   static void delete_genfitcLcLMeasurementOnPlane(void *p) {
      delete ((::genfit::MeasurementOnPlane*)p);
   }
   static void deleteArray_genfitcLcLMeasurementOnPlane(void *p) {
      delete [] ((::genfit::MeasurementOnPlane*)p);
   }
   static void destruct_genfitcLcLMeasurementOnPlane(void *p) {
      typedef ::genfit::MeasurementOnPlane current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_genfitcLcLMeasurementOnPlane(TBuffer &buf, void *obj) {
      ((::genfit::MeasurementOnPlane*)obj)->::genfit::MeasurementOnPlane::Streamer(buf);
   }
} // end of namespace ROOT for class ::genfit::MeasurementOnPlane

namespace ROOT {
   // Wrappers around operator new
   static void *new_genfitcLcLPruneFlags(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::genfit::PruneFlags : new ::genfit::PruneFlags;
   }
   static void *newArray_genfitcLcLPruneFlags(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::genfit::PruneFlags[nElements] : new ::genfit::PruneFlags[nElements];
   }
   // Wrapper around operator delete
   static void delete_genfitcLcLPruneFlags(void *p) {
      delete ((::genfit::PruneFlags*)p);
   }
   static void deleteArray_genfitcLcLPruneFlags(void *p) {
      delete [] ((::genfit::PruneFlags*)p);
   }
   static void destruct_genfitcLcLPruneFlags(void *p) {
      typedef ::genfit::PruneFlags current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::genfit::PruneFlags

namespace genfit {
//______________________________________________________________________________
void FitStatus::Streamer(TBuffer &R__b)
{
   // Stream an object of class genfit::FitStatus.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(genfit::FitStatus::Class(),this);
   } else {
      R__b.WriteClassBuffer(genfit::FitStatus::Class(),this);
   }
}

} // namespace genfit
namespace ROOT {
   // Wrappers around operator new
   static void *new_genfitcLcLFitStatus(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::genfit::FitStatus : new ::genfit::FitStatus;
   }
   static void *newArray_genfitcLcLFitStatus(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::genfit::FitStatus[nElements] : new ::genfit::FitStatus[nElements];
   }
   // Wrapper around operator delete
   static void delete_genfitcLcLFitStatus(void *p) {
      delete ((::genfit::FitStatus*)p);
   }
   static void deleteArray_genfitcLcLFitStatus(void *p) {
      delete [] ((::genfit::FitStatus*)p);
   }
   static void destruct_genfitcLcLFitStatus(void *p) {
      typedef ::genfit::FitStatus current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::genfit::FitStatus

namespace ROOT {
   // Wrapper around operator delete
   static void delete_genfitcLcLAbsFitterInfo(void *p) {
      delete ((::genfit::AbsFitterInfo*)p);
   }
   static void deleteArray_genfitcLcLAbsFitterInfo(void *p) {
      delete [] ((::genfit::AbsFitterInfo*)p);
   }
   static void destruct_genfitcLcLAbsFitterInfo(void *p) {
      typedef ::genfit::AbsFitterInfo current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_genfitcLcLAbsFitterInfo(TBuffer &buf, void *obj) {
      ((::genfit::AbsFitterInfo*)obj)->::genfit::AbsFitterInfo::Streamer(buf);
   }
} // end of namespace ROOT for class ::genfit::AbsFitterInfo

namespace genfit {
//______________________________________________________________________________
void TrackCandHit::Streamer(TBuffer &R__b)
{
   // Stream an object of class genfit::TrackCandHit.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(genfit::TrackCandHit::Class(),this);
   } else {
      R__b.WriteClassBuffer(genfit::TrackCandHit::Class(),this);
   }
}

} // namespace genfit
namespace ROOT {
   // Wrappers around operator new
   static void *new_genfitcLcLTrackCandHit(void *p) {
      return  p ? new(p) ::genfit::TrackCandHit : new ::genfit::TrackCandHit;
   }
   static void *newArray_genfitcLcLTrackCandHit(Long_t nElements, void *p) {
      return p ? new(p) ::genfit::TrackCandHit[nElements] : new ::genfit::TrackCandHit[nElements];
   }
   // Wrapper around operator delete
   static void delete_genfitcLcLTrackCandHit(void *p) {
      delete ((::genfit::TrackCandHit*)p);
   }
   static void deleteArray_genfitcLcLTrackCandHit(void *p) {
      delete [] ((::genfit::TrackCandHit*)p);
   }
   static void destruct_genfitcLcLTrackCandHit(void *p) {
      typedef ::genfit::TrackCandHit current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::genfit::TrackCandHit

namespace genfit {
//______________________________________________________________________________
void TrackCand::Streamer(TBuffer &R__b)
{
   // Stream an object of class genfit::TrackCand.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(genfit::TrackCand::Class(),this);
   } else {
      R__b.WriteClassBuffer(genfit::TrackCand::Class(),this);
   }
}

} // namespace genfit
namespace ROOT {
   // Wrappers around operator new
   static void *new_genfitcLcLTrackCand(void *p) {
      return  p ? new(p) ::genfit::TrackCand : new ::genfit::TrackCand;
   }
   static void *newArray_genfitcLcLTrackCand(Long_t nElements, void *p) {
      return p ? new(p) ::genfit::TrackCand[nElements] : new ::genfit::TrackCand[nElements];
   }
   // Wrapper around operator delete
   static void delete_genfitcLcLTrackCand(void *p) {
      delete ((::genfit::TrackCand*)p);
   }
   static void deleteArray_genfitcLcLTrackCand(void *p) {
      delete [] ((::genfit::TrackCand*)p);
   }
   static void destruct_genfitcLcLTrackCand(void *p) {
      typedef ::genfit::TrackCand current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::genfit::TrackCand

namespace genfit {
//______________________________________________________________________________
void AbsMeasurement::Streamer(TBuffer &R__b)
{
   // Stream an object of class genfit::AbsMeasurement.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(genfit::AbsMeasurement::Class(),this);
   } else {
      R__b.WriteClassBuffer(genfit::AbsMeasurement::Class(),this);
   }
}

} // namespace genfit
namespace ROOT {
   // Wrapper around operator delete
   static void delete_genfitcLcLAbsMeasurement(void *p) {
      delete ((::genfit::AbsMeasurement*)p);
   }
   static void deleteArray_genfitcLcLAbsMeasurement(void *p) {
      delete [] ((::genfit::AbsMeasurement*)p);
   }
   static void destruct_genfitcLcLAbsMeasurement(void *p) {
      typedef ::genfit::AbsMeasurement current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::genfit::AbsMeasurement

namespace ROOT {
   // Wrappers around operator new
   static void *new_genfitcLcLThinScatterer(void *p) {
      return  p ? new(p) ::genfit::ThinScatterer : new ::genfit::ThinScatterer;
   }
   static void *newArray_genfitcLcLThinScatterer(Long_t nElements, void *p) {
      return p ? new(p) ::genfit::ThinScatterer[nElements] : new ::genfit::ThinScatterer[nElements];
   }
   // Wrapper around operator delete
   static void delete_genfitcLcLThinScatterer(void *p) {
      delete ((::genfit::ThinScatterer*)p);
   }
   static void deleteArray_genfitcLcLThinScatterer(void *p) {
      delete [] ((::genfit::ThinScatterer*)p);
   }
   static void destruct_genfitcLcLThinScatterer(void *p) {
      typedef ::genfit::ThinScatterer current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_genfitcLcLThinScatterer(TBuffer &buf, void *obj) {
      ((::genfit::ThinScatterer*)obj)->::genfit::ThinScatterer::Streamer(buf);
   }
} // end of namespace ROOT for class ::genfit::ThinScatterer

namespace ROOT {
   // Wrappers around operator new
   static void *new_genfitcLcLTrackPoint(void *p) {
      return  p ? new(p) ::genfit::TrackPoint : new ::genfit::TrackPoint;
   }
   static void *newArray_genfitcLcLTrackPoint(Long_t nElements, void *p) {
      return p ? new(p) ::genfit::TrackPoint[nElements] : new ::genfit::TrackPoint[nElements];
   }
   // Wrapper around operator delete
   static void delete_genfitcLcLTrackPoint(void *p) {
      delete ((::genfit::TrackPoint*)p);
   }
   static void deleteArray_genfitcLcLTrackPoint(void *p) {
      delete [] ((::genfit::TrackPoint*)p);
   }
   static void destruct_genfitcLcLTrackPoint(void *p) {
      typedef ::genfit::TrackPoint current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_genfitcLcLTrackPoint(TBuffer &buf, void *obj) {
      ((::genfit::TrackPoint*)obj)->::genfit::TrackPoint::Streamer(buf);
   }
} // end of namespace ROOT for class ::genfit::TrackPoint

namespace ROOT {
   // Wrappers around operator new
   static void *new_genfitcLcLTrack(void *p) {
      return  p ? new(p) ::genfit::Track : new ::genfit::Track;
   }
   static void *newArray_genfitcLcLTrack(Long_t nElements, void *p) {
      return p ? new(p) ::genfit::Track[nElements] : new ::genfit::Track[nElements];
   }
   // Wrapper around operator delete
   static void delete_genfitcLcLTrack(void *p) {
      delete ((::genfit::Track*)p);
   }
   static void deleteArray_genfitcLcLTrack(void *p) {
      delete [] ((::genfit::Track*)p);
   }
   static void destruct_genfitcLcLTrack(void *p) {
      typedef ::genfit::Track current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_genfitcLcLTrack(TBuffer &buf, void *obj) {
      ((::genfit::Track*)obj)->::genfit::Track::Streamer(buf);
   }
} // end of namespace ROOT for class ::genfit::Track

namespace ROOT {
   static TClass *vectorlEgenfitcLcLTrackPointmUgR_Dictionary();
   static void vectorlEgenfitcLcLTrackPointmUgR_TClassManip(TClass*);
   static void *new_vectorlEgenfitcLcLTrackPointmUgR(void *p = nullptr);
   static void *newArray_vectorlEgenfitcLcLTrackPointmUgR(Long_t size, void *p);
   static void delete_vectorlEgenfitcLcLTrackPointmUgR(void *p);
   static void deleteArray_vectorlEgenfitcLcLTrackPointmUgR(void *p);
   static void destruct_vectorlEgenfitcLcLTrackPointmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<genfit::TrackPoint*>*)
   {
      vector<genfit::TrackPoint*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<genfit::TrackPoint*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<genfit::TrackPoint*>", -2, "vector", 389,
                  typeid(vector<genfit::TrackPoint*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEgenfitcLcLTrackPointmUgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<genfit::TrackPoint*>) );
      instance.SetNew(&new_vectorlEgenfitcLcLTrackPointmUgR);
      instance.SetNewArray(&newArray_vectorlEgenfitcLcLTrackPointmUgR);
      instance.SetDelete(&delete_vectorlEgenfitcLcLTrackPointmUgR);
      instance.SetDeleteArray(&deleteArray_vectorlEgenfitcLcLTrackPointmUgR);
      instance.SetDestructor(&destruct_vectorlEgenfitcLcLTrackPointmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<genfit::TrackPoint*> >()));

      ::ROOT::AddClassAlternate("vector<genfit::TrackPoint*>","std::vector<genfit::TrackPoint*, std::allocator<genfit::TrackPoint*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<genfit::TrackPoint*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEgenfitcLcLTrackPointmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<genfit::TrackPoint*>*)nullptr)->GetClass();
      vectorlEgenfitcLcLTrackPointmUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEgenfitcLcLTrackPointmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEgenfitcLcLTrackPointmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<genfit::TrackPoint*> : new vector<genfit::TrackPoint*>;
   }
   static void *newArray_vectorlEgenfitcLcLTrackPointmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<genfit::TrackPoint*>[nElements] : new vector<genfit::TrackPoint*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEgenfitcLcLTrackPointmUgR(void *p) {
      delete ((vector<genfit::TrackPoint*>*)p);
   }
   static void deleteArray_vectorlEgenfitcLcLTrackPointmUgR(void *p) {
      delete [] ((vector<genfit::TrackPoint*>*)p);
   }
   static void destruct_vectorlEgenfitcLcLTrackPointmUgR(void *p) {
      typedef vector<genfit::TrackPoint*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<genfit::TrackPoint*>

namespace ROOT {
   static TClass *vectorlEgenfitcLcLTrackCandHitmUgR_Dictionary();
   static void vectorlEgenfitcLcLTrackCandHitmUgR_TClassManip(TClass*);
   static void *new_vectorlEgenfitcLcLTrackCandHitmUgR(void *p = nullptr);
   static void *newArray_vectorlEgenfitcLcLTrackCandHitmUgR(Long_t size, void *p);
   static void delete_vectorlEgenfitcLcLTrackCandHitmUgR(void *p);
   static void deleteArray_vectorlEgenfitcLcLTrackCandHitmUgR(void *p);
   static void destruct_vectorlEgenfitcLcLTrackCandHitmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<genfit::TrackCandHit*>*)
   {
      vector<genfit::TrackCandHit*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<genfit::TrackCandHit*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<genfit::TrackCandHit*>", -2, "vector", 389,
                  typeid(vector<genfit::TrackCandHit*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEgenfitcLcLTrackCandHitmUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<genfit::TrackCandHit*>) );
      instance.SetNew(&new_vectorlEgenfitcLcLTrackCandHitmUgR);
      instance.SetNewArray(&newArray_vectorlEgenfitcLcLTrackCandHitmUgR);
      instance.SetDelete(&delete_vectorlEgenfitcLcLTrackCandHitmUgR);
      instance.SetDeleteArray(&deleteArray_vectorlEgenfitcLcLTrackCandHitmUgR);
      instance.SetDestructor(&destruct_vectorlEgenfitcLcLTrackCandHitmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<genfit::TrackCandHit*> >()));

      ::ROOT::AddClassAlternate("vector<genfit::TrackCandHit*>","std::vector<genfit::TrackCandHit*, std::allocator<genfit::TrackCandHit*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<genfit::TrackCandHit*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEgenfitcLcLTrackCandHitmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<genfit::TrackCandHit*>*)nullptr)->GetClass();
      vectorlEgenfitcLcLTrackCandHitmUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEgenfitcLcLTrackCandHitmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEgenfitcLcLTrackCandHitmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<genfit::TrackCandHit*> : new vector<genfit::TrackCandHit*>;
   }
   static void *newArray_vectorlEgenfitcLcLTrackCandHitmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<genfit::TrackCandHit*>[nElements] : new vector<genfit::TrackCandHit*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEgenfitcLcLTrackCandHitmUgR(void *p) {
      delete ((vector<genfit::TrackCandHit*>*)p);
   }
   static void deleteArray_vectorlEgenfitcLcLTrackCandHitmUgR(void *p) {
      delete [] ((vector<genfit::TrackCandHit*>*)p);
   }
   static void destruct_vectorlEgenfitcLcLTrackCandHitmUgR(void *p) {
      typedef vector<genfit::TrackCandHit*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<genfit::TrackCandHit*>

namespace ROOT {
   static TClass *vectorlEgenfitcLcLAbsTrackRepmUgR_Dictionary();
   static void vectorlEgenfitcLcLAbsTrackRepmUgR_TClassManip(TClass*);
   static void *new_vectorlEgenfitcLcLAbsTrackRepmUgR(void *p = nullptr);
   static void *newArray_vectorlEgenfitcLcLAbsTrackRepmUgR(Long_t size, void *p);
   static void delete_vectorlEgenfitcLcLAbsTrackRepmUgR(void *p);
   static void deleteArray_vectorlEgenfitcLcLAbsTrackRepmUgR(void *p);
   static void destruct_vectorlEgenfitcLcLAbsTrackRepmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<genfit::AbsTrackRep*>*)
   {
      vector<genfit::AbsTrackRep*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<genfit::AbsTrackRep*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<genfit::AbsTrackRep*>", -2, "vector", 389,
                  typeid(vector<genfit::AbsTrackRep*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEgenfitcLcLAbsTrackRepmUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<genfit::AbsTrackRep*>) );
      instance.SetNew(&new_vectorlEgenfitcLcLAbsTrackRepmUgR);
      instance.SetNewArray(&newArray_vectorlEgenfitcLcLAbsTrackRepmUgR);
      instance.SetDelete(&delete_vectorlEgenfitcLcLAbsTrackRepmUgR);
      instance.SetDeleteArray(&deleteArray_vectorlEgenfitcLcLAbsTrackRepmUgR);
      instance.SetDestructor(&destruct_vectorlEgenfitcLcLAbsTrackRepmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<genfit::AbsTrackRep*> >()));

      ::ROOT::AddClassAlternate("vector<genfit::AbsTrackRep*>","std::vector<genfit::AbsTrackRep*, std::allocator<genfit::AbsTrackRep*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<genfit::AbsTrackRep*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEgenfitcLcLAbsTrackRepmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<genfit::AbsTrackRep*>*)nullptr)->GetClass();
      vectorlEgenfitcLcLAbsTrackRepmUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEgenfitcLcLAbsTrackRepmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEgenfitcLcLAbsTrackRepmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<genfit::AbsTrackRep*> : new vector<genfit::AbsTrackRep*>;
   }
   static void *newArray_vectorlEgenfitcLcLAbsTrackRepmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<genfit::AbsTrackRep*>[nElements] : new vector<genfit::AbsTrackRep*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEgenfitcLcLAbsTrackRepmUgR(void *p) {
      delete ((vector<genfit::AbsTrackRep*>*)p);
   }
   static void deleteArray_vectorlEgenfitcLcLAbsTrackRepmUgR(void *p) {
      delete [] ((vector<genfit::AbsTrackRep*>*)p);
   }
   static void destruct_vectorlEgenfitcLcLAbsTrackRepmUgR(void *p) {
      typedef vector<genfit::AbsTrackRep*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<genfit::AbsTrackRep*>

namespace ROOT {
   static TClass *vectorlEgenfitcLcLAbsMeasurementmUgR_Dictionary();
   static void vectorlEgenfitcLcLAbsMeasurementmUgR_TClassManip(TClass*);
   static void *new_vectorlEgenfitcLcLAbsMeasurementmUgR(void *p = nullptr);
   static void *newArray_vectorlEgenfitcLcLAbsMeasurementmUgR(Long_t size, void *p);
   static void delete_vectorlEgenfitcLcLAbsMeasurementmUgR(void *p);
   static void deleteArray_vectorlEgenfitcLcLAbsMeasurementmUgR(void *p);
   static void destruct_vectorlEgenfitcLcLAbsMeasurementmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<genfit::AbsMeasurement*>*)
   {
      vector<genfit::AbsMeasurement*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<genfit::AbsMeasurement*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<genfit::AbsMeasurement*>", -2, "vector", 389,
                  typeid(vector<genfit::AbsMeasurement*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEgenfitcLcLAbsMeasurementmUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<genfit::AbsMeasurement*>) );
      instance.SetNew(&new_vectorlEgenfitcLcLAbsMeasurementmUgR);
      instance.SetNewArray(&newArray_vectorlEgenfitcLcLAbsMeasurementmUgR);
      instance.SetDelete(&delete_vectorlEgenfitcLcLAbsMeasurementmUgR);
      instance.SetDeleteArray(&deleteArray_vectorlEgenfitcLcLAbsMeasurementmUgR);
      instance.SetDestructor(&destruct_vectorlEgenfitcLcLAbsMeasurementmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<genfit::AbsMeasurement*> >()));

      ::ROOT::AddClassAlternate("vector<genfit::AbsMeasurement*>","std::vector<genfit::AbsMeasurement*, std::allocator<genfit::AbsMeasurement*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<genfit::AbsMeasurement*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEgenfitcLcLAbsMeasurementmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<genfit::AbsMeasurement*>*)nullptr)->GetClass();
      vectorlEgenfitcLcLAbsMeasurementmUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEgenfitcLcLAbsMeasurementmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEgenfitcLcLAbsMeasurementmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<genfit::AbsMeasurement*> : new vector<genfit::AbsMeasurement*>;
   }
   static void *newArray_vectorlEgenfitcLcLAbsMeasurementmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<genfit::AbsMeasurement*>[nElements] : new vector<genfit::AbsMeasurement*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEgenfitcLcLAbsMeasurementmUgR(void *p) {
      delete ((vector<genfit::AbsMeasurement*>*)p);
   }
   static void deleteArray_vectorlEgenfitcLcLAbsMeasurementmUgR(void *p) {
      delete [] ((vector<genfit::AbsMeasurement*>*)p);
   }
   static void destruct_vectorlEgenfitcLcLAbsMeasurementmUgR(void *p) {
      typedef vector<genfit::AbsMeasurement*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<genfit::AbsMeasurement*>

namespace ROOT {
   static TClass *maplEconstsPgenfitcLcLAbsTrackRepmUcOgenfitcLcLFitStatusmUgR_Dictionary();
   static void maplEconstsPgenfitcLcLAbsTrackRepmUcOgenfitcLcLFitStatusmUgR_TClassManip(TClass*);
   static void *new_maplEconstsPgenfitcLcLAbsTrackRepmUcOgenfitcLcLFitStatusmUgR(void *p = nullptr);
   static void *newArray_maplEconstsPgenfitcLcLAbsTrackRepmUcOgenfitcLcLFitStatusmUgR(Long_t size, void *p);
   static void delete_maplEconstsPgenfitcLcLAbsTrackRepmUcOgenfitcLcLFitStatusmUgR(void *p);
   static void deleteArray_maplEconstsPgenfitcLcLAbsTrackRepmUcOgenfitcLcLFitStatusmUgR(void *p);
   static void destruct_maplEconstsPgenfitcLcLAbsTrackRepmUcOgenfitcLcLFitStatusmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<const genfit::AbsTrackRep*,genfit::FitStatus*>*)
   {
      map<const genfit::AbsTrackRep*,genfit::FitStatus*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<const genfit::AbsTrackRep*,genfit::FitStatus*>));
      static ::ROOT::TGenericClassInfo 
         instance("map<const genfit::AbsTrackRep*,genfit::FitStatus*>", -2, "map", 100,
                  typeid(map<const genfit::AbsTrackRep*,genfit::FitStatus*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEconstsPgenfitcLcLAbsTrackRepmUcOgenfitcLcLFitStatusmUgR_Dictionary, isa_proxy, 0,
                  sizeof(map<const genfit::AbsTrackRep*,genfit::FitStatus*>) );
      instance.SetNew(&new_maplEconstsPgenfitcLcLAbsTrackRepmUcOgenfitcLcLFitStatusmUgR);
      instance.SetNewArray(&newArray_maplEconstsPgenfitcLcLAbsTrackRepmUcOgenfitcLcLFitStatusmUgR);
      instance.SetDelete(&delete_maplEconstsPgenfitcLcLAbsTrackRepmUcOgenfitcLcLFitStatusmUgR);
      instance.SetDeleteArray(&deleteArray_maplEconstsPgenfitcLcLAbsTrackRepmUcOgenfitcLcLFitStatusmUgR);
      instance.SetDestructor(&destruct_maplEconstsPgenfitcLcLAbsTrackRepmUcOgenfitcLcLFitStatusmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<const genfit::AbsTrackRep*,genfit::FitStatus*> >()));

      ::ROOT::AddClassAlternate("map<const genfit::AbsTrackRep*,genfit::FitStatus*>","std::map<genfit::AbsTrackRep const*, genfit::FitStatus*, std::less<genfit::AbsTrackRep const*>, std::allocator<std::pair<genfit::AbsTrackRep const* const, genfit::FitStatus*> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<const genfit::AbsTrackRep*,genfit::FitStatus*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEconstsPgenfitcLcLAbsTrackRepmUcOgenfitcLcLFitStatusmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<const genfit::AbsTrackRep*,genfit::FitStatus*>*)nullptr)->GetClass();
      maplEconstsPgenfitcLcLAbsTrackRepmUcOgenfitcLcLFitStatusmUgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEconstsPgenfitcLcLAbsTrackRepmUcOgenfitcLcLFitStatusmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEconstsPgenfitcLcLAbsTrackRepmUcOgenfitcLcLFitStatusmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<const genfit::AbsTrackRep*,genfit::FitStatus*> : new map<const genfit::AbsTrackRep*,genfit::FitStatus*>;
   }
   static void *newArray_maplEconstsPgenfitcLcLAbsTrackRepmUcOgenfitcLcLFitStatusmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<const genfit::AbsTrackRep*,genfit::FitStatus*>[nElements] : new map<const genfit::AbsTrackRep*,genfit::FitStatus*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEconstsPgenfitcLcLAbsTrackRepmUcOgenfitcLcLFitStatusmUgR(void *p) {
      delete ((map<const genfit::AbsTrackRep*,genfit::FitStatus*>*)p);
   }
   static void deleteArray_maplEconstsPgenfitcLcLAbsTrackRepmUcOgenfitcLcLFitStatusmUgR(void *p) {
      delete [] ((map<const genfit::AbsTrackRep*,genfit::FitStatus*>*)p);
   }
   static void destruct_maplEconstsPgenfitcLcLAbsTrackRepmUcOgenfitcLcLFitStatusmUgR(void *p) {
      typedef map<const genfit::AbsTrackRep*,genfit::FitStatus*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<const genfit::AbsTrackRep*,genfit::FitStatus*>

namespace {
  void TriggerDictionaryInitialization_libcoreRootDict_Impl() {
    static const char* headers[] = {
"/tmp/source/GenFit2/core/include/AbsFinitePlane.h",
"/tmp/source/GenFit2/core/include/AbsFitterInfo.h",
"/tmp/source/GenFit2/core/include/AbsHMatrix.h",
"/tmp/source/GenFit2/core/include/AbsTrackRep.h",
"/tmp/source/GenFit2/core/include/DetPlane.h",
"/tmp/source/GenFit2/core/include/FitStatus.h",
"/tmp/source/GenFit2/core/include/IO.h",
"/tmp/source/GenFit2/core/include/Material.h",
"/tmp/source/GenFit2/core/include/MeasuredStateOnPlane.h",
"/tmp/source/GenFit2/core/include/MeasurementFactory.h",
"/tmp/source/GenFit2/core/include/MeasurementOnPlane.h",
"/tmp/source/GenFit2/core/include/MeasurementProducer.h",
"/tmp/source/GenFit2/core/include/StateOnPlane.h",
"/tmp/source/GenFit2/core/include/AbsMeasurement.h",
"/tmp/source/GenFit2/core/include/ThinScatterer.h",
"/tmp/source/GenFit2/core/include/Track.h",
"/tmp/source/GenFit2/core/include/TrackCand.h",
"/tmp/source/GenFit2/core/include/TrackCandHit.h",
"/tmp/source/GenFit2/core/include/TrackPoint.h",
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
#line 1 "libcoreRootDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
namespace genfit{class __attribute__((annotate("$clingAutoload$TrackPoint.h")))  __attribute__((annotate("$clingAutoload$/tmp/source/GenFit2/core/include/Track.h")))  TrackPoint;}
namespace std{template <typename _Tp> class __attribute__((annotate("$clingAutoload$bits/allocator.h")))  __attribute__((annotate("$clingAutoload$string")))  allocator;
}
namespace genfit{class __attribute__((annotate("$clingAutoload$/tmp/source/GenFit2/core/include/AbsFinitePlane.h")))  AbsFinitePlane;}
namespace genfit{class __attribute__((annotate("$clingAutoload$DetPlane.h")))  __attribute__((annotate("$clingAutoload$/tmp/source/GenFit2/core/include/AbsFitterInfo.h")))  DetPlane;}
namespace genfit{class __attribute__((annotate("$clingAutoload$SharedPlanePtr.h")))  __attribute__((annotate("$clingAutoload$/tmp/source/GenFit2/core/include/AbsFitterInfo.h")))  SharedPlanePtrCreator;}
namespace genfit{struct __attribute__((annotate("$clingAutoload$Material.h")))  __attribute__((annotate("$clingAutoload$/tmp/source/GenFit2/core/include/AbsFitterInfo.h")))  Material;}
namespace genfit{class __attribute__((annotate("$clingAutoload$AbsTrackRep.h")))  __attribute__((annotate("$clingAutoload$/tmp/source/GenFit2/core/include/AbsFitterInfo.h")))  AbsTrackRep;}
namespace genfit{class __attribute__((annotate("$clingAutoload$StateOnPlane.h")))  __attribute__((annotate("$clingAutoload$/tmp/source/GenFit2/core/include/AbsFitterInfo.h")))  StateOnPlane;}
namespace genfit{class __attribute__((annotate("$clingAutoload$MeasuredStateOnPlane.h")))  __attribute__((annotate("$clingAutoload$/tmp/source/GenFit2/core/include/AbsFitterInfo.h")))  MeasuredStateOnPlane;}
namespace genfit{class __attribute__((annotate("$clingAutoload$AbsHMatrix.h")))  __attribute__((annotate("$clingAutoload$/tmp/source/GenFit2/core/include/AbsFitterInfo.h")))  AbsHMatrix;}
namespace genfit{class __attribute__((annotate("$clingAutoload$MeasurementOnPlane.h")))  __attribute__((annotate("$clingAutoload$/tmp/source/GenFit2/core/include/AbsFitterInfo.h")))  MeasurementOnPlane;}
namespace genfit{struct __attribute__((annotate("$clingAutoload$FitStatus.h")))  __attribute__((annotate("$clingAutoload$/tmp/source/GenFit2/core/include/AbsFitterInfo.h")))  PruneFlags;}
namespace genfit{class __attribute__((annotate("$clingAutoload$FitStatus.h")))  __attribute__((annotate("$clingAutoload$/tmp/source/GenFit2/core/include/AbsFitterInfo.h")))  FitStatus;}
namespace genfit{class __attribute__((annotate("$clingAutoload$/tmp/source/GenFit2/core/include/AbsFitterInfo.h")))  AbsFitterInfo;}
namespace genfit{class __attribute__((annotate("$clingAutoload$TrackCandHit.h")))  __attribute__((annotate("$clingAutoload$/tmp/source/GenFit2/core/include/MeasurementFactory.h")))  TrackCandHit;}
namespace genfit{class __attribute__((annotate("$clingAutoload$TrackCand.h")))  __attribute__((annotate("$clingAutoload$/tmp/source/GenFit2/core/include/MeasurementFactory.h")))  TrackCand;}
namespace genfit{class __attribute__((annotate("$clingAutoload$/tmp/source/GenFit2/core/include/AbsMeasurement.h")))  AbsMeasurement;}
namespace genfit{class __attribute__((annotate("$clingAutoload$/tmp/source/GenFit2/core/include/ThinScatterer.h")))  ThinScatterer;}
namespace genfit{class __attribute__((annotate("$clingAutoload$/tmp/source/GenFit2/core/include/Track.h")))  Track;}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libcoreRootDict dictionary payload"

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
#include "/tmp/source/GenFit2/core/include/AbsFinitePlane.h"
#include "/tmp/source/GenFit2/core/include/AbsFitterInfo.h"
#include "/tmp/source/GenFit2/core/include/AbsHMatrix.h"
#include "/tmp/source/GenFit2/core/include/AbsTrackRep.h"
#include "/tmp/source/GenFit2/core/include/DetPlane.h"
#include "/tmp/source/GenFit2/core/include/FitStatus.h"
#include "/tmp/source/GenFit2/core/include/IO.h"
#include "/tmp/source/GenFit2/core/include/Material.h"
#include "/tmp/source/GenFit2/core/include/MeasuredStateOnPlane.h"
#include "/tmp/source/GenFit2/core/include/MeasurementFactory.h"
#include "/tmp/source/GenFit2/core/include/MeasurementOnPlane.h"
#include "/tmp/source/GenFit2/core/include/MeasurementProducer.h"
#include "/tmp/source/GenFit2/core/include/StateOnPlane.h"
#include "/tmp/source/GenFit2/core/include/AbsMeasurement.h"
#include "/tmp/source/GenFit2/core/include/ThinScatterer.h"
#include "/tmp/source/GenFit2/core/include/Track.h"
#include "/tmp/source/GenFit2/core/include/TrackCand.h"
#include "/tmp/source/GenFit2/core/include/TrackCandHit.h"
#include "/tmp/source/GenFit2/core/include/TrackPoint.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"genfit::AbsFinitePlane", payloadCode, "@",
"genfit::AbsFitterInfo", payloadCode, "@",
"genfit::AbsHMatrix", payloadCode, "@",
"genfit::AbsMeasurement", payloadCode, "@",
"genfit::AbsTrackRep", payloadCode, "@",
"genfit::DetPlane", payloadCode, "@",
"genfit::FitStatus", payloadCode, "@",
"genfit::Material", payloadCode, "@",
"genfit::MeasuredStateOnPlane", payloadCode, "@",
"genfit::MeasurementOnPlane", payloadCode, "@",
"genfit::PruneFlags", payloadCode, "@",
"genfit::SharedPlanePtrCreator", payloadCode, "@",
"genfit::StateOnPlane", payloadCode, "@",
"genfit::ThinScatterer", payloadCode, "@",
"genfit::Track", payloadCode, "@",
"genfit::TrackCand", payloadCode, "@",
"genfit::TrackCandHit", payloadCode, "@",
"genfit::TrackPoint", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libcoreRootDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libcoreRootDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libcoreRootDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libcoreRootDict() {
  TriggerDictionaryInitialization_libcoreRootDict_Impl();
}
