// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME fittersRootDict
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
#include "/tmp/source/GenFit2/fitters/include/AbsKalmanFitter.h"
#include "/tmp/source/GenFit2/fitters/include/KalmanFittedStateOnPlane.h"
#include "/tmp/source/GenFit2/fitters/include/KalmanFitStatus.h"
#include "/tmp/source/GenFit2/fitters/include/ReferenceStateOnPlane.h"
#include "/tmp/source/GenFit2/fitters/include/KalmanFitterRefTrack.h"
#include "/tmp/source/GenFit2/fitters/include/KalmanFitter.h"
#include "/tmp/source/GenFit2/fitters/include/KalmanFitterInfo.h"
#include "/tmp/source/GenFit2/fitters/include/DAF.h"

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
            instance("genfit", 0 /*version*/, "AbsFitter.h", 27,
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
   static void delete_genfitcLcLAbsKalmanFitter(void *p);
   static void deleteArray_genfitcLcLAbsKalmanFitter(void *p);
   static void destruct_genfitcLcLAbsKalmanFitter(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::genfit::AbsKalmanFitter*)
   {
      ::genfit::AbsKalmanFitter *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::genfit::AbsKalmanFitter >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("genfit::AbsKalmanFitter", ::genfit::AbsKalmanFitter::Class_Version(), "AbsKalmanFitter.h", 51,
                  typeid(::genfit::AbsKalmanFitter), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::genfit::AbsKalmanFitter::Dictionary, isa_proxy, 4,
                  sizeof(::genfit::AbsKalmanFitter) );
      instance.SetDelete(&delete_genfitcLcLAbsKalmanFitter);
      instance.SetDeleteArray(&deleteArray_genfitcLcLAbsKalmanFitter);
      instance.SetDestructor(&destruct_genfitcLcLAbsKalmanFitter);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::genfit::AbsKalmanFitter*)
   {
      return GenerateInitInstanceLocal((::genfit::AbsKalmanFitter*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::genfit::AbsKalmanFitter*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_genfitcLcLKalmanFittedStateOnPlane(void *p = nullptr);
   static void *newArray_genfitcLcLKalmanFittedStateOnPlane(Long_t size, void *p);
   static void delete_genfitcLcLKalmanFittedStateOnPlane(void *p);
   static void deleteArray_genfitcLcLKalmanFittedStateOnPlane(void *p);
   static void destruct_genfitcLcLKalmanFittedStateOnPlane(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::genfit::KalmanFittedStateOnPlane*)
   {
      ::genfit::KalmanFittedStateOnPlane *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::genfit::KalmanFittedStateOnPlane >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("genfit::KalmanFittedStateOnPlane", ::genfit::KalmanFittedStateOnPlane::Class_Version(), "KalmanFittedStateOnPlane.h", 35,
                  typeid(::genfit::KalmanFittedStateOnPlane), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::genfit::KalmanFittedStateOnPlane::Dictionary, isa_proxy, 4,
                  sizeof(::genfit::KalmanFittedStateOnPlane) );
      instance.SetNew(&new_genfitcLcLKalmanFittedStateOnPlane);
      instance.SetNewArray(&newArray_genfitcLcLKalmanFittedStateOnPlane);
      instance.SetDelete(&delete_genfitcLcLKalmanFittedStateOnPlane);
      instance.SetDeleteArray(&deleteArray_genfitcLcLKalmanFittedStateOnPlane);
      instance.SetDestructor(&destruct_genfitcLcLKalmanFittedStateOnPlane);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::genfit::KalmanFittedStateOnPlane*)
   {
      return GenerateInitInstanceLocal((::genfit::KalmanFittedStateOnPlane*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::genfit::KalmanFittedStateOnPlane*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_genfitcLcLKalmanFitStatus(void *p = nullptr);
   static void *newArray_genfitcLcLKalmanFitStatus(Long_t size, void *p);
   static void delete_genfitcLcLKalmanFitStatus(void *p);
   static void deleteArray_genfitcLcLKalmanFitStatus(void *p);
   static void destruct_genfitcLcLKalmanFitStatus(void *p);
   static void streamer_genfitcLcLKalmanFitStatus(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::genfit::KalmanFitStatus*)
   {
      ::genfit::KalmanFitStatus *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::genfit::KalmanFitStatus >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("genfit::KalmanFitStatus", ::genfit::KalmanFitStatus::Class_Version(), "KalmanFitStatus.h", 36,
                  typeid(::genfit::KalmanFitStatus), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::genfit::KalmanFitStatus::Dictionary, isa_proxy, 16,
                  sizeof(::genfit::KalmanFitStatus) );
      instance.SetNew(&new_genfitcLcLKalmanFitStatus);
      instance.SetNewArray(&newArray_genfitcLcLKalmanFitStatus);
      instance.SetDelete(&delete_genfitcLcLKalmanFitStatus);
      instance.SetDeleteArray(&deleteArray_genfitcLcLKalmanFitStatus);
      instance.SetDestructor(&destruct_genfitcLcLKalmanFitStatus);
      instance.SetStreamerFunc(&streamer_genfitcLcLKalmanFitStatus);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::genfit::KalmanFitStatus*)
   {
      return GenerateInitInstanceLocal((::genfit::KalmanFitStatus*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::genfit::KalmanFitStatus*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_genfitcLcLReferenceStateOnPlane(void *p = nullptr);
   static void *newArray_genfitcLcLReferenceStateOnPlane(Long_t size, void *p);
   static void delete_genfitcLcLReferenceStateOnPlane(void *p);
   static void deleteArray_genfitcLcLReferenceStateOnPlane(void *p);
   static void destruct_genfitcLcLReferenceStateOnPlane(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::genfit::ReferenceStateOnPlane*)
   {
      ::genfit::ReferenceStateOnPlane *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::genfit::ReferenceStateOnPlane >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("genfit::ReferenceStateOnPlane", ::genfit::ReferenceStateOnPlane::Class_Version(), "ReferenceStateOnPlane.h", 43,
                  typeid(::genfit::ReferenceStateOnPlane), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::genfit::ReferenceStateOnPlane::Dictionary, isa_proxy, 4,
                  sizeof(::genfit::ReferenceStateOnPlane) );
      instance.SetNew(&new_genfitcLcLReferenceStateOnPlane);
      instance.SetNewArray(&newArray_genfitcLcLReferenceStateOnPlane);
      instance.SetDelete(&delete_genfitcLcLReferenceStateOnPlane);
      instance.SetDeleteArray(&deleteArray_genfitcLcLReferenceStateOnPlane);
      instance.SetDestructor(&destruct_genfitcLcLReferenceStateOnPlane);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::genfit::ReferenceStateOnPlane*)
   {
      return GenerateInitInstanceLocal((::genfit::ReferenceStateOnPlane*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::genfit::ReferenceStateOnPlane*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_genfitcLcLKalmanFitterRefTrack(void *p = nullptr);
   static void *newArray_genfitcLcLKalmanFitterRefTrack(Long_t size, void *p);
   static void delete_genfitcLcLKalmanFitterRefTrack(void *p);
   static void deleteArray_genfitcLcLKalmanFitterRefTrack(void *p);
   static void destruct_genfitcLcLKalmanFitterRefTrack(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::genfit::KalmanFitterRefTrack*)
   {
      ::genfit::KalmanFitterRefTrack *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::genfit::KalmanFitterRefTrack >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("genfit::KalmanFitterRefTrack", ::genfit::KalmanFitterRefTrack::Class_Version(), "KalmanFitterRefTrack.h", 37,
                  typeid(::genfit::KalmanFitterRefTrack), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::genfit::KalmanFitterRefTrack::Dictionary, isa_proxy, 4,
                  sizeof(::genfit::KalmanFitterRefTrack) );
      instance.SetNew(&new_genfitcLcLKalmanFitterRefTrack);
      instance.SetNewArray(&newArray_genfitcLcLKalmanFitterRefTrack);
      instance.SetDelete(&delete_genfitcLcLKalmanFitterRefTrack);
      instance.SetDeleteArray(&deleteArray_genfitcLcLKalmanFitterRefTrack);
      instance.SetDestructor(&destruct_genfitcLcLKalmanFitterRefTrack);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::genfit::KalmanFitterRefTrack*)
   {
      return GenerateInitInstanceLocal((::genfit::KalmanFitterRefTrack*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::genfit::KalmanFitterRefTrack*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_genfitcLcLKalmanFitter(void *p = nullptr);
   static void *newArray_genfitcLcLKalmanFitter(Long_t size, void *p);
   static void delete_genfitcLcLKalmanFitter(void *p);
   static void deleteArray_genfitcLcLKalmanFitter(void *p);
   static void destruct_genfitcLcLKalmanFitter(void *p);
   static void streamer_genfitcLcLKalmanFitter(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::genfit::KalmanFitter*)
   {
      ::genfit::KalmanFitter *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::genfit::KalmanFitter >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("genfit::KalmanFitter", ::genfit::KalmanFitter::Class_Version(), "KalmanFitter.h", 40,
                  typeid(::genfit::KalmanFitter), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::genfit::KalmanFitter::Dictionary, isa_proxy, 17,
                  sizeof(::genfit::KalmanFitter) );
      instance.SetNew(&new_genfitcLcLKalmanFitter);
      instance.SetNewArray(&newArray_genfitcLcLKalmanFitter);
      instance.SetDelete(&delete_genfitcLcLKalmanFitter);
      instance.SetDeleteArray(&deleteArray_genfitcLcLKalmanFitter);
      instance.SetDestructor(&destruct_genfitcLcLKalmanFitter);
      instance.SetStreamerFunc(&streamer_genfitcLcLKalmanFitter);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::genfit::KalmanFitter*)
   {
      return GenerateInitInstanceLocal((::genfit::KalmanFitter*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::genfit::KalmanFitter*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_genfitcLcLKalmanFitterInfo(void *p = nullptr);
   static void *newArray_genfitcLcLKalmanFitterInfo(Long_t size, void *p);
   static void delete_genfitcLcLKalmanFitterInfo(void *p);
   static void deleteArray_genfitcLcLKalmanFitterInfo(void *p);
   static void destruct_genfitcLcLKalmanFitterInfo(void *p);
   static void streamer_genfitcLcLKalmanFitterInfo(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::genfit::KalmanFitterInfo*)
   {
      ::genfit::KalmanFitterInfo *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::genfit::KalmanFitterInfo >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("genfit::KalmanFitterInfo", ::genfit::KalmanFitterInfo::Class_Version(), "KalmanFitterInfo.h", 44,
                  typeid(::genfit::KalmanFitterInfo), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::genfit::KalmanFitterInfo::Dictionary, isa_proxy, 17,
                  sizeof(::genfit::KalmanFitterInfo) );
      instance.SetNew(&new_genfitcLcLKalmanFitterInfo);
      instance.SetNewArray(&newArray_genfitcLcLKalmanFitterInfo);
      instance.SetDelete(&delete_genfitcLcLKalmanFitterInfo);
      instance.SetDeleteArray(&deleteArray_genfitcLcLKalmanFitterInfo);
      instance.SetDestructor(&destruct_genfitcLcLKalmanFitterInfo);
      instance.SetStreamerFunc(&streamer_genfitcLcLKalmanFitterInfo);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::genfit::KalmanFitterInfo*)
   {
      return GenerateInitInstanceLocal((::genfit::KalmanFitterInfo*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::genfit::KalmanFitterInfo*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_genfitcLcLDAF(void *p = nullptr);
   static void *newArray_genfitcLcLDAF(Long_t size, void *p);
   static void delete_genfitcLcLDAF(void *p);
   static void deleteArray_genfitcLcLDAF(void *p);
   static void destruct_genfitcLcLDAF(void *p);
   static void streamer_genfitcLcLDAF(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::genfit::DAF*)
   {
      ::genfit::DAF *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::genfit::DAF >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("genfit::DAF", ::genfit::DAF::Class_Version(), "DAF.h", 49,
                  typeid(::genfit::DAF), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::genfit::DAF::Dictionary, isa_proxy, 17,
                  sizeof(::genfit::DAF) );
      instance.SetNew(&new_genfitcLcLDAF);
      instance.SetNewArray(&newArray_genfitcLcLDAF);
      instance.SetDelete(&delete_genfitcLcLDAF);
      instance.SetDeleteArray(&deleteArray_genfitcLcLDAF);
      instance.SetDestructor(&destruct_genfitcLcLDAF);
      instance.SetStreamerFunc(&streamer_genfitcLcLDAF);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::genfit::DAF*)
   {
      return GenerateInitInstanceLocal((::genfit::DAF*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::genfit::DAF*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace genfit {
//______________________________________________________________________________
atomic_TClass_ptr AbsKalmanFitter::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *AbsKalmanFitter::Class_Name()
{
   return "genfit::AbsKalmanFitter";
}

//______________________________________________________________________________
const char *AbsKalmanFitter::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::AbsKalmanFitter*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int AbsKalmanFitter::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::AbsKalmanFitter*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *AbsKalmanFitter::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::AbsKalmanFitter*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *AbsKalmanFitter::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::AbsKalmanFitter*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace genfit
namespace genfit {
//______________________________________________________________________________
atomic_TClass_ptr KalmanFittedStateOnPlane::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *KalmanFittedStateOnPlane::Class_Name()
{
   return "genfit::KalmanFittedStateOnPlane";
}

//______________________________________________________________________________
const char *KalmanFittedStateOnPlane::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::KalmanFittedStateOnPlane*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int KalmanFittedStateOnPlane::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::KalmanFittedStateOnPlane*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *KalmanFittedStateOnPlane::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::KalmanFittedStateOnPlane*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *KalmanFittedStateOnPlane::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::KalmanFittedStateOnPlane*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace genfit
namespace genfit {
//______________________________________________________________________________
atomic_TClass_ptr KalmanFitStatus::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *KalmanFitStatus::Class_Name()
{
   return "genfit::KalmanFitStatus";
}

//______________________________________________________________________________
const char *KalmanFitStatus::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::KalmanFitStatus*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int KalmanFitStatus::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::KalmanFitStatus*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *KalmanFitStatus::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::KalmanFitStatus*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *KalmanFitStatus::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::KalmanFitStatus*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace genfit
namespace genfit {
//______________________________________________________________________________
atomic_TClass_ptr ReferenceStateOnPlane::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *ReferenceStateOnPlane::Class_Name()
{
   return "genfit::ReferenceStateOnPlane";
}

//______________________________________________________________________________
const char *ReferenceStateOnPlane::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::ReferenceStateOnPlane*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int ReferenceStateOnPlane::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::ReferenceStateOnPlane*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ReferenceStateOnPlane::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::ReferenceStateOnPlane*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ReferenceStateOnPlane::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::ReferenceStateOnPlane*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace genfit
namespace genfit {
//______________________________________________________________________________
atomic_TClass_ptr KalmanFitterRefTrack::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *KalmanFitterRefTrack::Class_Name()
{
   return "genfit::KalmanFitterRefTrack";
}

//______________________________________________________________________________
const char *KalmanFitterRefTrack::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::KalmanFitterRefTrack*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int KalmanFitterRefTrack::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::KalmanFitterRefTrack*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *KalmanFitterRefTrack::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::KalmanFitterRefTrack*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *KalmanFitterRefTrack::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::KalmanFitterRefTrack*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace genfit
namespace genfit {
//______________________________________________________________________________
atomic_TClass_ptr KalmanFitter::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *KalmanFitter::Class_Name()
{
   return "genfit::KalmanFitter";
}

//______________________________________________________________________________
const char *KalmanFitter::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::KalmanFitter*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int KalmanFitter::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::KalmanFitter*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *KalmanFitter::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::KalmanFitter*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *KalmanFitter::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::KalmanFitter*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace genfit
namespace genfit {
//______________________________________________________________________________
atomic_TClass_ptr KalmanFitterInfo::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *KalmanFitterInfo::Class_Name()
{
   return "genfit::KalmanFitterInfo";
}

//______________________________________________________________________________
const char *KalmanFitterInfo::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::KalmanFitterInfo*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int KalmanFitterInfo::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::KalmanFitterInfo*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *KalmanFitterInfo::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::KalmanFitterInfo*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *KalmanFitterInfo::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::KalmanFitterInfo*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace genfit
namespace genfit {
//______________________________________________________________________________
atomic_TClass_ptr DAF::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *DAF::Class_Name()
{
   return "genfit::DAF";
}

//______________________________________________________________________________
const char *DAF::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::DAF*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int DAF::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::genfit::DAF*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *DAF::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::DAF*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *DAF::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::genfit::DAF*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace genfit
namespace genfit {
//______________________________________________________________________________
void AbsKalmanFitter::Streamer(TBuffer &R__b)
{
   // Stream an object of class genfit::AbsKalmanFitter.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(genfit::AbsKalmanFitter::Class(),this);
   } else {
      R__b.WriteClassBuffer(genfit::AbsKalmanFitter::Class(),this);
   }
}

} // namespace genfit
namespace ROOT {
   // Wrapper around operator delete
   static void delete_genfitcLcLAbsKalmanFitter(void *p) {
      delete ((::genfit::AbsKalmanFitter*)p);
   }
   static void deleteArray_genfitcLcLAbsKalmanFitter(void *p) {
      delete [] ((::genfit::AbsKalmanFitter*)p);
   }
   static void destruct_genfitcLcLAbsKalmanFitter(void *p) {
      typedef ::genfit::AbsKalmanFitter current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::genfit::AbsKalmanFitter

namespace genfit {
//______________________________________________________________________________
void KalmanFittedStateOnPlane::Streamer(TBuffer &R__b)
{
   // Stream an object of class genfit::KalmanFittedStateOnPlane.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(genfit::KalmanFittedStateOnPlane::Class(),this);
   } else {
      R__b.WriteClassBuffer(genfit::KalmanFittedStateOnPlane::Class(),this);
   }
}

} // namespace genfit
namespace ROOT {
   // Wrappers around operator new
   static void *new_genfitcLcLKalmanFittedStateOnPlane(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::genfit::KalmanFittedStateOnPlane : new ::genfit::KalmanFittedStateOnPlane;
   }
   static void *newArray_genfitcLcLKalmanFittedStateOnPlane(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::genfit::KalmanFittedStateOnPlane[nElements] : new ::genfit::KalmanFittedStateOnPlane[nElements];
   }
   // Wrapper around operator delete
   static void delete_genfitcLcLKalmanFittedStateOnPlane(void *p) {
      delete ((::genfit::KalmanFittedStateOnPlane*)p);
   }
   static void deleteArray_genfitcLcLKalmanFittedStateOnPlane(void *p) {
      delete [] ((::genfit::KalmanFittedStateOnPlane*)p);
   }
   static void destruct_genfitcLcLKalmanFittedStateOnPlane(void *p) {
      typedef ::genfit::KalmanFittedStateOnPlane current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::genfit::KalmanFittedStateOnPlane

namespace genfit {
//______________________________________________________________________________
void KalmanFitStatus::Streamer(TBuffer &R__b)
{
   // Stream an object of class genfit::KalmanFitStatus.

   //This works around a msvc bug and should be harmless on other platforms
   typedef ::genfit::KalmanFitStatus thisClass;
   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      //This works around a msvc bug and should be harmless on other platforms
      typedef genfit::FitStatus baseClass0;
      baseClass0::Streamer(R__b);
      R__b >> numIterations_;
      R__b >> fittedWithDaf_;
      R__b >> fittedWithReferenceTrack_;
      R__b >> trackLen_;
      R__b >> fChi2_;
      R__b >> fNdf_;
      R__b >> fPval_;
      R__b.CheckByteCount(R__s, R__c, thisClass::IsA());
   } else {
      R__c = R__b.WriteVersion(thisClass::IsA(), kTRUE);
      //This works around a msvc bug and should be harmless on other platforms
      typedef genfit::FitStatus baseClass0;
      baseClass0::Streamer(R__b);
      R__b << numIterations_;
      R__b << fittedWithDaf_;
      R__b << fittedWithReferenceTrack_;
      R__b << trackLen_;
      R__b << fChi2_;
      R__b << fNdf_;
      R__b << fPval_;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

} // namespace genfit
namespace ROOT {
   // Wrappers around operator new
   static void *new_genfitcLcLKalmanFitStatus(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::genfit::KalmanFitStatus : new ::genfit::KalmanFitStatus;
   }
   static void *newArray_genfitcLcLKalmanFitStatus(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::genfit::KalmanFitStatus[nElements] : new ::genfit::KalmanFitStatus[nElements];
   }
   // Wrapper around operator delete
   static void delete_genfitcLcLKalmanFitStatus(void *p) {
      delete ((::genfit::KalmanFitStatus*)p);
   }
   static void deleteArray_genfitcLcLKalmanFitStatus(void *p) {
      delete [] ((::genfit::KalmanFitStatus*)p);
   }
   static void destruct_genfitcLcLKalmanFitStatus(void *p) {
      typedef ::genfit::KalmanFitStatus current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_genfitcLcLKalmanFitStatus(TBuffer &buf, void *obj) {
      ((::genfit::KalmanFitStatus*)obj)->::genfit::KalmanFitStatus::Streamer(buf);
   }
} // end of namespace ROOT for class ::genfit::KalmanFitStatus

namespace genfit {
//______________________________________________________________________________
void ReferenceStateOnPlane::Streamer(TBuffer &R__b)
{
   // Stream an object of class genfit::ReferenceStateOnPlane.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(genfit::ReferenceStateOnPlane::Class(),this);
   } else {
      R__b.WriteClassBuffer(genfit::ReferenceStateOnPlane::Class(),this);
   }
}

} // namespace genfit
namespace ROOT {
   // Wrappers around operator new
   static void *new_genfitcLcLReferenceStateOnPlane(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::genfit::ReferenceStateOnPlane : new ::genfit::ReferenceStateOnPlane;
   }
   static void *newArray_genfitcLcLReferenceStateOnPlane(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::genfit::ReferenceStateOnPlane[nElements] : new ::genfit::ReferenceStateOnPlane[nElements];
   }
   // Wrapper around operator delete
   static void delete_genfitcLcLReferenceStateOnPlane(void *p) {
      delete ((::genfit::ReferenceStateOnPlane*)p);
   }
   static void deleteArray_genfitcLcLReferenceStateOnPlane(void *p) {
      delete [] ((::genfit::ReferenceStateOnPlane*)p);
   }
   static void destruct_genfitcLcLReferenceStateOnPlane(void *p) {
      typedef ::genfit::ReferenceStateOnPlane current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::genfit::ReferenceStateOnPlane

namespace genfit {
//______________________________________________________________________________
void KalmanFitterRefTrack::Streamer(TBuffer &R__b)
{
   // Stream an object of class genfit::KalmanFitterRefTrack.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(genfit::KalmanFitterRefTrack::Class(),this);
   } else {
      R__b.WriteClassBuffer(genfit::KalmanFitterRefTrack::Class(),this);
   }
}

} // namespace genfit
namespace ROOT {
   // Wrappers around operator new
   static void *new_genfitcLcLKalmanFitterRefTrack(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::genfit::KalmanFitterRefTrack : new ::genfit::KalmanFitterRefTrack;
   }
   static void *newArray_genfitcLcLKalmanFitterRefTrack(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::genfit::KalmanFitterRefTrack[nElements] : new ::genfit::KalmanFitterRefTrack[nElements];
   }
   // Wrapper around operator delete
   static void delete_genfitcLcLKalmanFitterRefTrack(void *p) {
      delete ((::genfit::KalmanFitterRefTrack*)p);
   }
   static void deleteArray_genfitcLcLKalmanFitterRefTrack(void *p) {
      delete [] ((::genfit::KalmanFitterRefTrack*)p);
   }
   static void destruct_genfitcLcLKalmanFitterRefTrack(void *p) {
      typedef ::genfit::KalmanFitterRefTrack current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::genfit::KalmanFitterRefTrack

namespace ROOT {
   // Wrappers around operator new
   static void *new_genfitcLcLKalmanFitter(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::genfit::KalmanFitter : new ::genfit::KalmanFitter;
   }
   static void *newArray_genfitcLcLKalmanFitter(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::genfit::KalmanFitter[nElements] : new ::genfit::KalmanFitter[nElements];
   }
   // Wrapper around operator delete
   static void delete_genfitcLcLKalmanFitter(void *p) {
      delete ((::genfit::KalmanFitter*)p);
   }
   static void deleteArray_genfitcLcLKalmanFitter(void *p) {
      delete [] ((::genfit::KalmanFitter*)p);
   }
   static void destruct_genfitcLcLKalmanFitter(void *p) {
      typedef ::genfit::KalmanFitter current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_genfitcLcLKalmanFitter(TBuffer &buf, void *obj) {
      ((::genfit::KalmanFitter*)obj)->::genfit::KalmanFitter::Streamer(buf);
   }
} // end of namespace ROOT for class ::genfit::KalmanFitter

namespace ROOT {
   // Wrappers around operator new
   static void *new_genfitcLcLKalmanFitterInfo(void *p) {
      return  p ? new(p) ::genfit::KalmanFitterInfo : new ::genfit::KalmanFitterInfo;
   }
   static void *newArray_genfitcLcLKalmanFitterInfo(Long_t nElements, void *p) {
      return p ? new(p) ::genfit::KalmanFitterInfo[nElements] : new ::genfit::KalmanFitterInfo[nElements];
   }
   // Wrapper around operator delete
   static void delete_genfitcLcLKalmanFitterInfo(void *p) {
      delete ((::genfit::KalmanFitterInfo*)p);
   }
   static void deleteArray_genfitcLcLKalmanFitterInfo(void *p) {
      delete [] ((::genfit::KalmanFitterInfo*)p);
   }
   static void destruct_genfitcLcLKalmanFitterInfo(void *p) {
      typedef ::genfit::KalmanFitterInfo current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_genfitcLcLKalmanFitterInfo(TBuffer &buf, void *obj) {
      ((::genfit::KalmanFitterInfo*)obj)->::genfit::KalmanFitterInfo::Streamer(buf);
   }
} // end of namespace ROOT for class ::genfit::KalmanFitterInfo

namespace ROOT {
   // Wrappers around operator new
   static void *new_genfitcLcLDAF(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::genfit::DAF : new ::genfit::DAF;
   }
   static void *newArray_genfitcLcLDAF(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::genfit::DAF[nElements] : new ::genfit::DAF[nElements];
   }
   // Wrapper around operator delete
   static void delete_genfitcLcLDAF(void *p) {
      delete ((::genfit::DAF*)p);
   }
   static void deleteArray_genfitcLcLDAF(void *p) {
      delete [] ((::genfit::DAF*)p);
   }
   static void destruct_genfitcLcLDAF(void *p) {
      typedef ::genfit::DAF current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_genfitcLcLDAF(TBuffer &buf, void *obj) {
      ((::genfit::DAF*)obj)->::genfit::DAF::Streamer(buf);
   }
} // end of namespace ROOT for class ::genfit::DAF

namespace ROOT {
   static TClass *vectorlEgenfitcLcLMeasurementOnPlanemUgR_Dictionary();
   static void vectorlEgenfitcLcLMeasurementOnPlanemUgR_TClassManip(TClass*);
   static void *new_vectorlEgenfitcLcLMeasurementOnPlanemUgR(void *p = nullptr);
   static void *newArray_vectorlEgenfitcLcLMeasurementOnPlanemUgR(Long_t size, void *p);
   static void delete_vectorlEgenfitcLcLMeasurementOnPlanemUgR(void *p);
   static void deleteArray_vectorlEgenfitcLcLMeasurementOnPlanemUgR(void *p);
   static void destruct_vectorlEgenfitcLcLMeasurementOnPlanemUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<genfit::MeasurementOnPlane*>*)
   {
      vector<genfit::MeasurementOnPlane*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<genfit::MeasurementOnPlane*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<genfit::MeasurementOnPlane*>", -2, "vector", 389,
                  typeid(vector<genfit::MeasurementOnPlane*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEgenfitcLcLMeasurementOnPlanemUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<genfit::MeasurementOnPlane*>) );
      instance.SetNew(&new_vectorlEgenfitcLcLMeasurementOnPlanemUgR);
      instance.SetNewArray(&newArray_vectorlEgenfitcLcLMeasurementOnPlanemUgR);
      instance.SetDelete(&delete_vectorlEgenfitcLcLMeasurementOnPlanemUgR);
      instance.SetDeleteArray(&deleteArray_vectorlEgenfitcLcLMeasurementOnPlanemUgR);
      instance.SetDestructor(&destruct_vectorlEgenfitcLcLMeasurementOnPlanemUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<genfit::MeasurementOnPlane*> >()));

      ::ROOT::AddClassAlternate("vector<genfit::MeasurementOnPlane*>","std::vector<genfit::MeasurementOnPlane*, std::allocator<genfit::MeasurementOnPlane*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<genfit::MeasurementOnPlane*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEgenfitcLcLMeasurementOnPlanemUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<genfit::MeasurementOnPlane*>*)nullptr)->GetClass();
      vectorlEgenfitcLcLMeasurementOnPlanemUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEgenfitcLcLMeasurementOnPlanemUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEgenfitcLcLMeasurementOnPlanemUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<genfit::MeasurementOnPlane*> : new vector<genfit::MeasurementOnPlane*>;
   }
   static void *newArray_vectorlEgenfitcLcLMeasurementOnPlanemUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<genfit::MeasurementOnPlane*>[nElements] : new vector<genfit::MeasurementOnPlane*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEgenfitcLcLMeasurementOnPlanemUgR(void *p) {
      delete ((vector<genfit::MeasurementOnPlane*>*)p);
   }
   static void deleteArray_vectorlEgenfitcLcLMeasurementOnPlanemUgR(void *p) {
      delete [] ((vector<genfit::MeasurementOnPlane*>*)p);
   }
   static void destruct_vectorlEgenfitcLcLMeasurementOnPlanemUgR(void *p) {
      typedef vector<genfit::MeasurementOnPlane*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<genfit::MeasurementOnPlane*>

namespace ROOT {
   static TClass *vectorlEdoublegR_Dictionary();
   static void vectorlEdoublegR_TClassManip(TClass*);
   static void *new_vectorlEdoublegR(void *p = nullptr);
   static void *newArray_vectorlEdoublegR(Long_t size, void *p);
   static void delete_vectorlEdoublegR(void *p);
   static void deleteArray_vectorlEdoublegR(void *p);
   static void destruct_vectorlEdoublegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<double>*)
   {
      vector<double> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<double>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<double>", -2, "vector", 389,
                  typeid(vector<double>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEdoublegR_Dictionary, isa_proxy, 0,
                  sizeof(vector<double>) );
      instance.SetNew(&new_vectorlEdoublegR);
      instance.SetNewArray(&newArray_vectorlEdoublegR);
      instance.SetDelete(&delete_vectorlEdoublegR);
      instance.SetDeleteArray(&deleteArray_vectorlEdoublegR);
      instance.SetDestructor(&destruct_vectorlEdoublegR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<double> >()));

      ::ROOT::AddClassAlternate("vector<double>","std::vector<double, std::allocator<double> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<double>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEdoublegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<double>*)nullptr)->GetClass();
      vectorlEdoublegR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEdoublegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEdoublegR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<double> : new vector<double>;
   }
   static void *newArray_vectorlEdoublegR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<double>[nElements] : new vector<double>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEdoublegR(void *p) {
      delete ((vector<double>*)p);
   }
   static void deleteArray_vectorlEdoublegR(void *p) {
      delete [] ((vector<double>*)p);
   }
   static void destruct_vectorlEdoublegR(void *p) {
      typedef vector<double> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<double>

namespace {
  void TriggerDictionaryInitialization_libfittersRootDict_Impl() {
    static const char* headers[] = {
"/tmp/source/GenFit2/fitters/include/AbsKalmanFitter.h",
"/tmp/source/GenFit2/fitters/include/KalmanFittedStateOnPlane.h",
"/tmp/source/GenFit2/fitters/include/KalmanFitStatus.h",
"/tmp/source/GenFit2/fitters/include/ReferenceStateOnPlane.h",
"/tmp/source/GenFit2/fitters/include/KalmanFitterRefTrack.h",
"/tmp/source/GenFit2/fitters/include/KalmanFitter.h",
"/tmp/source/GenFit2/fitters/include/KalmanFitterInfo.h",
"/tmp/source/GenFit2/fitters/include/DAF.h",
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
#line 1 "libfittersRootDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
namespace genfit{class __attribute__((annotate("$clingAutoload$/tmp/source/GenFit2/fitters/include/AbsKalmanFitter.h")))  AbsKalmanFitter;}
namespace genfit{class __attribute__((annotate("$clingAutoload$/tmp/source/GenFit2/fitters/include/KalmanFittedStateOnPlane.h")))  KalmanFittedStateOnPlane;}
namespace genfit{class __attribute__((annotate("$clingAutoload$/tmp/source/GenFit2/fitters/include/KalmanFitStatus.h")))  KalmanFitStatus;}
namespace genfit{class __attribute__((annotate("$clingAutoload$/tmp/source/GenFit2/fitters/include/ReferenceStateOnPlane.h")))  ReferenceStateOnPlane;}
namespace genfit{class __attribute__((annotate("$clingAutoload$/tmp/source/GenFit2/fitters/include/KalmanFitterRefTrack.h")))  KalmanFitterRefTrack;}
namespace genfit{class __attribute__((annotate("$clingAutoload$/tmp/source/GenFit2/fitters/include/KalmanFitter.h")))  KalmanFitter;}
namespace genfit{class __attribute__((annotate("$clingAutoload$/tmp/source/GenFit2/fitters/include/KalmanFitterInfo.h")))  KalmanFitterInfo;}
namespace genfit{class __attribute__((annotate("$clingAutoload$/tmp/source/GenFit2/fitters/include/DAF.h")))  DAF;}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libfittersRootDict dictionary payload"

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
#include "/tmp/source/GenFit2/fitters/include/AbsKalmanFitter.h"
#include "/tmp/source/GenFit2/fitters/include/KalmanFittedStateOnPlane.h"
#include "/tmp/source/GenFit2/fitters/include/KalmanFitStatus.h"
#include "/tmp/source/GenFit2/fitters/include/ReferenceStateOnPlane.h"
#include "/tmp/source/GenFit2/fitters/include/KalmanFitterRefTrack.h"
#include "/tmp/source/GenFit2/fitters/include/KalmanFitter.h"
#include "/tmp/source/GenFit2/fitters/include/KalmanFitterInfo.h"
#include "/tmp/source/GenFit2/fitters/include/DAF.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"genfit::AbsKalmanFitter", payloadCode, "@",
"genfit::DAF", payloadCode, "@",
"genfit::KalmanFitStatus", payloadCode, "@",
"genfit::KalmanFittedStateOnPlane", payloadCode, "@",
"genfit::KalmanFitter", payloadCode, "@",
"genfit::KalmanFitterInfo", payloadCode, "@",
"genfit::KalmanFitterRefTrack", payloadCode, "@",
"genfit::ReferenceStateOnPlane", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libfittersRootDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libfittersRootDict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libfittersRootDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libfittersRootDict() {
  TriggerDictionaryInitialization_libfittersRootDict_Impl();
}
