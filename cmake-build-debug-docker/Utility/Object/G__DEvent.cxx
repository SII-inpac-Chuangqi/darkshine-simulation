// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__DEvent
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
#include "/tmp/source/Utility/Object/include/Object/DHit.h"
#include "/tmp/source/Utility/Object/include/Object/DParticle.h"
#include "/tmp/source/Utility/Object/include/Object/DStep.h"
#include "/tmp/source/Utility/Object/include/Object/SimulatedHit.h"
#include "/tmp/source/Utility/Object/include/Object/CalorimeterHit.h"
#include "/tmp/source/Utility/Object/include/Object/McParticle.h"
#include "/tmp/source/Utility/Object/include/Object/ReconstructedParticle.h"
#include "/tmp/source/Utility/Object/include/Object/DigiForm.hh"
#include "/tmp/source/Utility/Object/include/Object/DEvent.h"
#include "/tmp/source/Utility/Object/include/Object/DMagnet.h"
#include "/tmp/source/Utility/Object/include/Object/DDetectorIDMaps.h"
#include "/tmp/source/Utility/Object/include/Object/McPHelper.h"
#include "/tmp/source/Utility/Object/include/Object/DTruth.h"
#include "/tmp/source/Utility/Object/include/Object/versions/DHit_b1_4.h"
#include "/tmp/source/Utility/Object/include/Object/versions/DParticle_b1_5.h"
#include "/tmp/source/Utility/Object/include/Object/versions/DStep_b1_5.h"
#include "/tmp/source/Utility/Object/include/Object/versions/SimulatedHit_b1_5.h"
#include "/tmp/source/Utility/Object/include/Object/versions/CalorimeterHit_b1.h"
#include "/tmp/source/Utility/Object/include/Object/versions/McParticle_b1_5.h"
#include "/tmp/source/Utility/Object/include/Object/versions/ReconstructedParticle_b1_5.h"
#include "/tmp/source/Utility/Object/include/Object/versions/DigiForm_b1.hh"
#include "/tmp/source/Utility/Object/include/Object/versions/DEvent_b1_5.h"
#include "/tmp/source/Utility/Object/include/Object/versions/DMagnet_b1.h"
#include "/tmp/source/Utility/Object/include/Object/versions/DDetectorIDMaps_b1_5.h"
#include "/tmp/source/Utility/Object/include/Object/versions/McPHelper_b1_5.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void *new_DHit_b1_4(void *p = nullptr);
   static void *newArray_DHit_b1_4(Long_t size, void *p);
   static void delete_DHit_b1_4(void *p);
   static void deleteArray_DHit_b1_4(void *p);
   static void destruct_DHit_b1_4(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::DHit_b1_4*)
   {
      ::DHit_b1_4 *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::DHit_b1_4 >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("DHit_b1_4", ::DHit_b1_4::Class_Version(), "versions/DHit_b1_4.h", 14,
                  typeid(::DHit_b1_4), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::DHit_b1_4::Dictionary, isa_proxy, 4,
                  sizeof(::DHit_b1_4) );
      instance.SetNew(&new_DHit_b1_4);
      instance.SetNewArray(&newArray_DHit_b1_4);
      instance.SetDelete(&delete_DHit_b1_4);
      instance.SetDeleteArray(&deleteArray_DHit_b1_4);
      instance.SetDestructor(&destruct_DHit_b1_4);

      ::ROOT::AddClassAlternate("DHit_b1_4","DHit");
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::DHit_b1_4*)
   {
      return GenerateInitInstanceLocal((::DHit_b1_4*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::DHit_b1_4*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_DParticle_b1_5(void *p = nullptr);
   static void *newArray_DParticle_b1_5(Long_t size, void *p);
   static void delete_DParticle_b1_5(void *p);
   static void deleteArray_DParticle_b1_5(void *p);
   static void destruct_DParticle_b1_5(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::DParticle_b1_5*)
   {
      ::DParticle_b1_5 *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::DParticle_b1_5 >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("DParticle_b1_5", ::DParticle_b1_5::Class_Version(), "versions/DParticle_b1_5.h", 13,
                  typeid(::DParticle_b1_5), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::DParticle_b1_5::Dictionary, isa_proxy, 4,
                  sizeof(::DParticle_b1_5) );
      instance.SetNew(&new_DParticle_b1_5);
      instance.SetNewArray(&newArray_DParticle_b1_5);
      instance.SetDelete(&delete_DParticle_b1_5);
      instance.SetDeleteArray(&deleteArray_DParticle_b1_5);
      instance.SetDestructor(&destruct_DParticle_b1_5);

      ::ROOT::AddClassAlternate("DParticle_b1_5","DParticle");
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::DParticle_b1_5*)
   {
      return GenerateInitInstanceLocal((::DParticle_b1_5*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::DParticle_b1_5*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_DStep_b1_5(void *p = nullptr);
   static void *newArray_DStep_b1_5(Long_t size, void *p);
   static void delete_DStep_b1_5(void *p);
   static void deleteArray_DStep_b1_5(void *p);
   static void destruct_DStep_b1_5(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::DStep_b1_5*)
   {
      ::DStep_b1_5 *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::DStep_b1_5 >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("DStep_b1_5", ::DStep_b1_5::Class_Version(), "versions/DStep_b1_5.h", 14,
                  typeid(::DStep_b1_5), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::DStep_b1_5::Dictionary, isa_proxy, 4,
                  sizeof(::DStep_b1_5) );
      instance.SetNew(&new_DStep_b1_5);
      instance.SetNewArray(&newArray_DStep_b1_5);
      instance.SetDelete(&delete_DStep_b1_5);
      instance.SetDeleteArray(&deleteArray_DStep_b1_5);
      instance.SetDestructor(&destruct_DStep_b1_5);

      ::ROOT::AddClassAlternate("DStep_b1_5","DStep");
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::DStep_b1_5*)
   {
      return GenerateInitInstanceLocal((::DStep_b1_5*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::DStep_b1_5*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_CalorimeterHit_b1(void *p = nullptr);
   static void *newArray_CalorimeterHit_b1(Long_t size, void *p);
   static void delete_CalorimeterHit_b1(void *p);
   static void deleteArray_CalorimeterHit_b1(void *p);
   static void destruct_CalorimeterHit_b1(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::CalorimeterHit_b1*)
   {
      ::CalorimeterHit_b1 *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::CalorimeterHit_b1 >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("CalorimeterHit_b1", ::CalorimeterHit_b1::Class_Version(), "versions/CalorimeterHit_b1.h", 18,
                  typeid(::CalorimeterHit_b1), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::CalorimeterHit_b1::Dictionary, isa_proxy, 4,
                  sizeof(::CalorimeterHit_b1) );
      instance.SetNew(&new_CalorimeterHit_b1);
      instance.SetNewArray(&newArray_CalorimeterHit_b1);
      instance.SetDelete(&delete_CalorimeterHit_b1);
      instance.SetDeleteArray(&deleteArray_CalorimeterHit_b1);
      instance.SetDestructor(&destruct_CalorimeterHit_b1);

      ::ROOT::AddClassAlternate("CalorimeterHit_b1","CalorimeterHit");
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::CalorimeterHit_b1*)
   {
      return GenerateInitInstanceLocal((::CalorimeterHit_b1*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::CalorimeterHit_b1*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_ReconstructedParticle_b1_5(void *p = nullptr);
   static void *newArray_ReconstructedParticle_b1_5(Long_t size, void *p);
   static void delete_ReconstructedParticle_b1_5(void *p);
   static void deleteArray_ReconstructedParticle_b1_5(void *p);
   static void destruct_ReconstructedParticle_b1_5(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ReconstructedParticle_b1_5*)
   {
      ::ReconstructedParticle_b1_5 *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::ReconstructedParticle_b1_5 >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("ReconstructedParticle_b1_5", ::ReconstructedParticle_b1_5::Class_Version(), "versions/ReconstructedParticle_b1_5.h", 13,
                  typeid(::ReconstructedParticle_b1_5), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::ReconstructedParticle_b1_5::Dictionary, isa_proxy, 4,
                  sizeof(::ReconstructedParticle_b1_5) );
      instance.SetNew(&new_ReconstructedParticle_b1_5);
      instance.SetNewArray(&newArray_ReconstructedParticle_b1_5);
      instance.SetDelete(&delete_ReconstructedParticle_b1_5);
      instance.SetDeleteArray(&deleteArray_ReconstructedParticle_b1_5);
      instance.SetDestructor(&destruct_ReconstructedParticle_b1_5);

      ::ROOT::AddClassAlternate("ReconstructedParticle_b1_5","ReconstructedParticle");
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::ReconstructedParticle_b1_5*)
   {
      return GenerateInitInstanceLocal((::ReconstructedParticle_b1_5*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::ReconstructedParticle_b1_5*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_McParticle_b1_5(void *p = nullptr);
   static void *newArray_McParticle_b1_5(Long_t size, void *p);
   static void delete_McParticle_b1_5(void *p);
   static void deleteArray_McParticle_b1_5(void *p);
   static void destruct_McParticle_b1_5(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::McParticle_b1_5*)
   {
      ::McParticle_b1_5 *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::McParticle_b1_5 >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("McParticle_b1_5", ::McParticle_b1_5::Class_Version(), "versions/McParticle_b1_5.h", 15,
                  typeid(::McParticle_b1_5), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::McParticle_b1_5::Dictionary, isa_proxy, 4,
                  sizeof(::McParticle_b1_5) );
      instance.SetNew(&new_McParticle_b1_5);
      instance.SetNewArray(&newArray_McParticle_b1_5);
      instance.SetDelete(&delete_McParticle_b1_5);
      instance.SetDeleteArray(&deleteArray_McParticle_b1_5);
      instance.SetDestructor(&destruct_McParticle_b1_5);

      ::ROOT::AddClassAlternate("McParticle_b1_5","McParticle");
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::McParticle_b1_5*)
   {
      return GenerateInitInstanceLocal((::McParticle_b1_5*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::McParticle_b1_5*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_SimulatedHit_b1_5(void *p = nullptr);
   static void *newArray_SimulatedHit_b1_5(Long_t size, void *p);
   static void delete_SimulatedHit_b1_5(void *p);
   static void deleteArray_SimulatedHit_b1_5(void *p);
   static void destruct_SimulatedHit_b1_5(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::SimulatedHit_b1_5*)
   {
      ::SimulatedHit_b1_5 *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::SimulatedHit_b1_5 >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("SimulatedHit_b1_5", ::SimulatedHit_b1_5::Class_Version(), "versions/SimulatedHit_b1_5.h", 18,
                  typeid(::SimulatedHit_b1_5), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::SimulatedHit_b1_5::Dictionary, isa_proxy, 4,
                  sizeof(::SimulatedHit_b1_5) );
      instance.SetNew(&new_SimulatedHit_b1_5);
      instance.SetNewArray(&newArray_SimulatedHit_b1_5);
      instance.SetDelete(&delete_SimulatedHit_b1_5);
      instance.SetDeleteArray(&deleteArray_SimulatedHit_b1_5);
      instance.SetDestructor(&destruct_SimulatedHit_b1_5);

      ::ROOT::AddClassAlternate("SimulatedHit_b1_5","SimulatedHit");
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::SimulatedHit_b1_5*)
   {
      return GenerateInitInstanceLocal((::SimulatedHit_b1_5*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::SimulatedHit_b1_5*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_DigiForm_b1(void *p = nullptr);
   static void *newArray_DigiForm_b1(Long_t size, void *p);
   static void delete_DigiForm_b1(void *p);
   static void deleteArray_DigiForm_b1(void *p);
   static void destruct_DigiForm_b1(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::DigiForm_b1*)
   {
      ::DigiForm_b1 *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::DigiForm_b1 >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("DigiForm_b1", ::DigiForm_b1::Class_Version(), "versions/DigiForm_b1.hh", 41,
                  typeid(::DigiForm_b1), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::DigiForm_b1::Dictionary, isa_proxy, 4,
                  sizeof(::DigiForm_b1) );
      instance.SetNew(&new_DigiForm_b1);
      instance.SetNewArray(&newArray_DigiForm_b1);
      instance.SetDelete(&delete_DigiForm_b1);
      instance.SetDeleteArray(&deleteArray_DigiForm_b1);
      instance.SetDestructor(&destruct_DigiForm_b1);

      ::ROOT::AddClassAlternate("DigiForm_b1","DigiForm");
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::DigiForm_b1*)
   {
      return GenerateInitInstanceLocal((::DigiForm_b1*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::DigiForm_b1*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_McPHelper_b1_5(void *p = nullptr);
   static void *newArray_McPHelper_b1_5(Long_t size, void *p);
   static void delete_McPHelper_b1_5(void *p);
   static void deleteArray_McPHelper_b1_5(void *p);
   static void destruct_McPHelper_b1_5(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::McPHelper_b1_5*)
   {
      ::McPHelper_b1_5 *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::McPHelper_b1_5 >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("McPHelper_b1_5", ::McPHelper_b1_5::Class_Version(), "versions/McPHelper_b1_5.h", 11,
                  typeid(::McPHelper_b1_5), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::McPHelper_b1_5::Dictionary, isa_proxy, 4,
                  sizeof(::McPHelper_b1_5) );
      instance.SetNew(&new_McPHelper_b1_5);
      instance.SetNewArray(&newArray_McPHelper_b1_5);
      instance.SetDelete(&delete_McPHelper_b1_5);
      instance.SetDeleteArray(&deleteArray_McPHelper_b1_5);
      instance.SetDestructor(&destruct_McPHelper_b1_5);

      ::ROOT::AddClassAlternate("McPHelper_b1_5","McPHelper");
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::McPHelper_b1_5*)
   {
      return GenerateInitInstanceLocal((::McPHelper_b1_5*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::McPHelper_b1_5*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *PhysicsDef_Dictionary();
   static void PhysicsDef_TClassManip(TClass*);
   static void *new_PhysicsDef(void *p = nullptr);
   static void *newArray_PhysicsDef(Long_t size, void *p);
   static void delete_PhysicsDef(void *p);
   static void deleteArray_PhysicsDef(void *p);
   static void destruct_PhysicsDef(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PhysicsDef*)
   {
      ::PhysicsDef *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::PhysicsDef));
      static ::ROOT::TGenericClassInfo 
         instance("PhysicsDef", "Utility/PhysicsProcessDef.h", 16,
                  typeid(::PhysicsDef), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &PhysicsDef_Dictionary, isa_proxy, 4,
                  sizeof(::PhysicsDef) );
      instance.SetNew(&new_PhysicsDef);
      instance.SetNewArray(&newArray_PhysicsDef);
      instance.SetDelete(&delete_PhysicsDef);
      instance.SetDeleteArray(&deleteArray_PhysicsDef);
      instance.SetDestructor(&destruct_PhysicsDef);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PhysicsDef*)
   {
      return GenerateInitInstanceLocal((::PhysicsDef*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::PhysicsDef*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *PhysicsDef_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::PhysicsDef*)nullptr)->GetClass();
      PhysicsDef_TClassManip(theClass);
   return theClass;
   }

   static void PhysicsDef_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void *new_DTruthParticle(void *p = nullptr);
   static void *newArray_DTruthParticle(Long_t size, void *p);
   static void delete_DTruthParticle(void *p);
   static void deleteArray_DTruthParticle(void *p);
   static void destruct_DTruthParticle(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::DTruthParticle*)
   {
      ::DTruthParticle *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::DTruthParticle >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("DTruthParticle", ::DTruthParticle::Class_Version(), "DTruth.h", 32,
                  typeid(::DTruthParticle), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::DTruthParticle::Dictionary, isa_proxy, 4,
                  sizeof(::DTruthParticle) );
      instance.SetNew(&new_DTruthParticle);
      instance.SetNewArray(&newArray_DTruthParticle);
      instance.SetDelete(&delete_DTruthParticle);
      instance.SetDeleteArray(&deleteArray_DTruthParticle);
      instance.SetDestructor(&destruct_DTruthParticle);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::DTruthParticle*)
   {
      return GenerateInitInstanceLocal((::DTruthParticle*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::DTruthParticle*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_DTruthProcess(void *p = nullptr);
   static void *newArray_DTruthProcess(Long_t size, void *p);
   static void delete_DTruthProcess(void *p);
   static void deleteArray_DTruthProcess(void *p);
   static void destruct_DTruthProcess(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::DTruthProcess*)
   {
      ::DTruthProcess *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::DTruthProcess >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("DTruthProcess", ::DTruthProcess::Class_Version(), "DTruth.h", 67,
                  typeid(::DTruthProcess), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::DTruthProcess::Dictionary, isa_proxy, 4,
                  sizeof(::DTruthProcess) );
      instance.SetNew(&new_DTruthProcess);
      instance.SetNewArray(&newArray_DTruthProcess);
      instance.SetDelete(&delete_DTruthProcess);
      instance.SetDeleteArray(&deleteArray_DTruthProcess);
      instance.SetDestructor(&destruct_DTruthProcess);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::DTruthProcess*)
   {
      return GenerateInitInstanceLocal((::DTruthProcess*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::DTruthProcess*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_DTruthState(void *p = nullptr);
   static void *newArray_DTruthState(Long_t size, void *p);
   static void delete_DTruthState(void *p);
   static void deleteArray_DTruthState(void *p);
   static void destruct_DTruthState(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::DTruthState*)
   {
      ::DTruthState *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::DTruthState >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("DTruthState", ::DTruthState::Class_Version(), "DTruth.h", 84,
                  typeid(::DTruthState), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::DTruthState::Dictionary, isa_proxy, 4,
                  sizeof(::DTruthState) );
      instance.SetNew(&new_DTruthState);
      instance.SetNewArray(&newArray_DTruthState);
      instance.SetDelete(&delete_DTruthState);
      instance.SetDeleteArray(&deleteArray_DTruthState);
      instance.SetDestructor(&destruct_DTruthState);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::DTruthState*)
   {
      return GenerateInitInstanceLocal((::DTruthState*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::DTruthState*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_DTruth(void *p = nullptr);
   static void *newArray_DTruth(Long_t size, void *p);
   static void delete_DTruth(void *p);
   static void deleteArray_DTruth(void *p);
   static void destruct_DTruth(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::DTruth*)
   {
      ::DTruth *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::DTruth >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("DTruth", ::DTruth::Class_Version(), "DTruth.h", 117,
                  typeid(::DTruth), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::DTruth::Dictionary, isa_proxy, 4,
                  sizeof(::DTruth) );
      instance.SetNew(&new_DTruth);
      instance.SetNewArray(&newArray_DTruth);
      instance.SetDelete(&delete_DTruth);
      instance.SetDeleteArray(&deleteArray_DTruth);
      instance.SetDestructor(&destruct_DTruth);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::DTruth*)
   {
      return GenerateInitInstanceLocal((::DTruth*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::DTruth*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_DEvent_b1_5(void *p = nullptr);
   static void *newArray_DEvent_b1_5(Long_t size, void *p);
   static void delete_DEvent_b1_5(void *p);
   static void deleteArray_DEvent_b1_5(void *p);
   static void destruct_DEvent_b1_5(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::DEvent_b1_5*)
   {
      ::DEvent_b1_5 *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::DEvent_b1_5 >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("DEvent_b1_5", ::DEvent_b1_5::Class_Version(), "versions/DEvent_b1_5.h", 34,
                  typeid(::DEvent_b1_5), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::DEvent_b1_5::Dictionary, isa_proxy, 4,
                  sizeof(::DEvent_b1_5) );
      instance.SetNew(&new_DEvent_b1_5);
      instance.SetNewArray(&newArray_DEvent_b1_5);
      instance.SetDelete(&delete_DEvent_b1_5);
      instance.SetDeleteArray(&deleteArray_DEvent_b1_5);
      instance.SetDestructor(&destruct_DEvent_b1_5);

      ::ROOT::AddClassAlternate("DEvent_b1_5","DEvent");
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::DEvent_b1_5*)
   {
      return GenerateInitInstanceLocal((::DEvent_b1_5*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::DEvent_b1_5*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_DMagnet_b1(void *p = nullptr);
   static void *newArray_DMagnet_b1(Long_t size, void *p);
   static void delete_DMagnet_b1(void *p);
   static void deleteArray_DMagnet_b1(void *p);
   static void destruct_DMagnet_b1(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::DMagnet_b1*)
   {
      ::DMagnet_b1 *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::DMagnet_b1 >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("DMagnet_b1", ::DMagnet_b1::Class_Version(), "versions/DMagnet_b1.h", 10,
                  typeid(::DMagnet_b1), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::DMagnet_b1::Dictionary, isa_proxy, 4,
                  sizeof(::DMagnet_b1) );
      instance.SetNew(&new_DMagnet_b1);
      instance.SetNewArray(&newArray_DMagnet_b1);
      instance.SetDelete(&delete_DMagnet_b1);
      instance.SetDeleteArray(&deleteArray_DMagnet_b1);
      instance.SetDestructor(&destruct_DMagnet_b1);

      ::ROOT::AddClassAlternate("DMagnet_b1","DMagnet");
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::DMagnet_b1*)
   {
      return GenerateInitInstanceLocal((::DMagnet_b1*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::DMagnet_b1*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static TClass *DMagnet_b1cLcLMagSlice_Dictionary();
   static void DMagnet_b1cLcLMagSlice_TClassManip(TClass*);
   static void *new_DMagnet_b1cLcLMagSlice(void *p = nullptr);
   static void *newArray_DMagnet_b1cLcLMagSlice(Long_t size, void *p);
   static void delete_DMagnet_b1cLcLMagSlice(void *p);
   static void deleteArray_DMagnet_b1cLcLMagSlice(void *p);
   static void destruct_DMagnet_b1cLcLMagSlice(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::DMagnet_b1::MagSlice*)
   {
      ::DMagnet_b1::MagSlice *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::DMagnet_b1::MagSlice));
      static ::ROOT::TGenericClassInfo 
         instance("DMagnet_b1::MagSlice", "versions/DMagnet_b1.h", 16,
                  typeid(::DMagnet_b1::MagSlice), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &DMagnet_b1cLcLMagSlice_Dictionary, isa_proxy, 4,
                  sizeof(::DMagnet_b1::MagSlice) );
      instance.SetNew(&new_DMagnet_b1cLcLMagSlice);
      instance.SetNewArray(&newArray_DMagnet_b1cLcLMagSlice);
      instance.SetDelete(&delete_DMagnet_b1cLcLMagSlice);
      instance.SetDeleteArray(&deleteArray_DMagnet_b1cLcLMagSlice);
      instance.SetDestructor(&destruct_DMagnet_b1cLcLMagSlice);

      ::ROOT::AddClassAlternate("DMagnet_b1::MagSlice","DMagnet::MagSlice");
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::DMagnet_b1::MagSlice*)
   {
      return GenerateInitInstanceLocal((::DMagnet_b1::MagSlice*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::DMagnet_b1::MagSlice*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *DMagnet_b1cLcLMagSlice_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::DMagnet_b1::MagSlice*)nullptr)->GetClass();
      DMagnet_b1cLcLMagSlice_TClassManip(theClass);
   return theClass;
   }

   static void DMagnet_b1cLcLMagSlice_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void delete_DDetectorIDMaps_b1_5(void *p);
   static void deleteArray_DDetectorIDMaps_b1_5(void *p);
   static void destruct_DDetectorIDMaps_b1_5(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::DDetectorIDMaps_b1_5*)
   {
      ::DDetectorIDMaps_b1_5 *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::DDetectorIDMaps_b1_5 >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("DDetectorIDMaps_b1_5", ::DDetectorIDMaps_b1_5::Class_Version(), "versions/DDetectorIDMaps_b1_5.h", 12,
                  typeid(::DDetectorIDMaps_b1_5), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::DDetectorIDMaps_b1_5::Dictionary, isa_proxy, 4,
                  sizeof(::DDetectorIDMaps_b1_5) );
      instance.SetDelete(&delete_DDetectorIDMaps_b1_5);
      instance.SetDeleteArray(&deleteArray_DDetectorIDMaps_b1_5);
      instance.SetDestructor(&destruct_DDetectorIDMaps_b1_5);

      ::ROOT::AddClassAlternate("DDetectorIDMaps_b1_5","DDetectorIDMaps");
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::DDetectorIDMaps_b1_5*)
   {
      return GenerateInitInstanceLocal((::DDetectorIDMaps_b1_5*)nullptr);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::DDetectorIDMaps_b1_5*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr DHit_b1_4::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *DHit_b1_4::Class_Name()
{
   return "DHit_b1_4";
}

//______________________________________________________________________________
const char *DHit_b1_4::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DHit_b1_4*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int DHit_b1_4::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DHit_b1_4*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *DHit_b1_4::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DHit_b1_4*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *DHit_b1_4::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DHit_b1_4*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr DParticle_b1_5::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *DParticle_b1_5::Class_Name()
{
   return "DParticle_b1_5";
}

//______________________________________________________________________________
const char *DParticle_b1_5::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DParticle_b1_5*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int DParticle_b1_5::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DParticle_b1_5*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *DParticle_b1_5::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DParticle_b1_5*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *DParticle_b1_5::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DParticle_b1_5*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr DStep_b1_5::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *DStep_b1_5::Class_Name()
{
   return "DStep_b1_5";
}

//______________________________________________________________________________
const char *DStep_b1_5::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DStep_b1_5*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int DStep_b1_5::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DStep_b1_5*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *DStep_b1_5::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DStep_b1_5*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *DStep_b1_5::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DStep_b1_5*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr CalorimeterHit_b1::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *CalorimeterHit_b1::Class_Name()
{
   return "CalorimeterHit_b1";
}

//______________________________________________________________________________
const char *CalorimeterHit_b1::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CalorimeterHit_b1*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int CalorimeterHit_b1::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::CalorimeterHit_b1*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *CalorimeterHit_b1::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CalorimeterHit_b1*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *CalorimeterHit_b1::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::CalorimeterHit_b1*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr ReconstructedParticle_b1_5::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *ReconstructedParticle_b1_5::Class_Name()
{
   return "ReconstructedParticle_b1_5";
}

//______________________________________________________________________________
const char *ReconstructedParticle_b1_5::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ReconstructedParticle_b1_5*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int ReconstructedParticle_b1_5::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::ReconstructedParticle_b1_5*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *ReconstructedParticle_b1_5::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ReconstructedParticle_b1_5*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *ReconstructedParticle_b1_5::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::ReconstructedParticle_b1_5*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr McParticle_b1_5::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *McParticle_b1_5::Class_Name()
{
   return "McParticle_b1_5";
}

//______________________________________________________________________________
const char *McParticle_b1_5::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::McParticle_b1_5*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int McParticle_b1_5::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::McParticle_b1_5*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *McParticle_b1_5::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::McParticle_b1_5*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *McParticle_b1_5::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::McParticle_b1_5*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr SimulatedHit_b1_5::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *SimulatedHit_b1_5::Class_Name()
{
   return "SimulatedHit_b1_5";
}

//______________________________________________________________________________
const char *SimulatedHit_b1_5::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::SimulatedHit_b1_5*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int SimulatedHit_b1_5::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::SimulatedHit_b1_5*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *SimulatedHit_b1_5::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::SimulatedHit_b1_5*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *SimulatedHit_b1_5::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::SimulatedHit_b1_5*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr DigiForm_b1::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *DigiForm_b1::Class_Name()
{
   return "DigiForm_b1";
}

//______________________________________________________________________________
const char *DigiForm_b1::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DigiForm_b1*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int DigiForm_b1::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DigiForm_b1*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *DigiForm_b1::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DigiForm_b1*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *DigiForm_b1::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DigiForm_b1*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr McPHelper_b1_5::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *McPHelper_b1_5::Class_Name()
{
   return "McPHelper_b1_5";
}

//______________________________________________________________________________
const char *McPHelper_b1_5::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::McPHelper_b1_5*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int McPHelper_b1_5::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::McPHelper_b1_5*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *McPHelper_b1_5::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::McPHelper_b1_5*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *McPHelper_b1_5::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::McPHelper_b1_5*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr DTruthParticle::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *DTruthParticle::Class_Name()
{
   return "DTruthParticle";
}

//______________________________________________________________________________
const char *DTruthParticle::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DTruthParticle*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int DTruthParticle::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DTruthParticle*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *DTruthParticle::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DTruthParticle*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *DTruthParticle::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DTruthParticle*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr DTruthProcess::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *DTruthProcess::Class_Name()
{
   return "DTruthProcess";
}

//______________________________________________________________________________
const char *DTruthProcess::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DTruthProcess*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int DTruthProcess::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DTruthProcess*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *DTruthProcess::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DTruthProcess*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *DTruthProcess::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DTruthProcess*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr DTruthState::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *DTruthState::Class_Name()
{
   return "DTruthState";
}

//______________________________________________________________________________
const char *DTruthState::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DTruthState*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int DTruthState::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DTruthState*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *DTruthState::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DTruthState*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *DTruthState::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DTruthState*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr DTruth::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *DTruth::Class_Name()
{
   return "DTruth";
}

//______________________________________________________________________________
const char *DTruth::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DTruth*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int DTruth::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DTruth*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *DTruth::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DTruth*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *DTruth::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DTruth*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr DEvent_b1_5::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *DEvent_b1_5::Class_Name()
{
   return "DEvent_b1_5";
}

//______________________________________________________________________________
const char *DEvent_b1_5::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DEvent_b1_5*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int DEvent_b1_5::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DEvent_b1_5*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *DEvent_b1_5::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DEvent_b1_5*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *DEvent_b1_5::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DEvent_b1_5*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr DMagnet_b1::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *DMagnet_b1::Class_Name()
{
   return "DMagnet_b1";
}

//______________________________________________________________________________
const char *DMagnet_b1::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DMagnet_b1*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int DMagnet_b1::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DMagnet_b1*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *DMagnet_b1::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DMagnet_b1*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *DMagnet_b1::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DMagnet_b1*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr DDetectorIDMaps_b1_5::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *DDetectorIDMaps_b1_5::Class_Name()
{
   return "DDetectorIDMaps_b1_5";
}

//______________________________________________________________________________
const char *DDetectorIDMaps_b1_5::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DDetectorIDMaps_b1_5*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int DDetectorIDMaps_b1_5::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::DDetectorIDMaps_b1_5*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *DDetectorIDMaps_b1_5::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DDetectorIDMaps_b1_5*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *DDetectorIDMaps_b1_5::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::DDetectorIDMaps_b1_5*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void DHit_b1_4::Streamer(TBuffer &R__b)
{
   // Stream an object of class DHit_b1_4.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(DHit_b1_4::Class(),this);
   } else {
      R__b.WriteClassBuffer(DHit_b1_4::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_DHit_b1_4(void *p) {
      return  p ? new(p) ::DHit_b1_4 : new ::DHit_b1_4;
   }
   static void *newArray_DHit_b1_4(Long_t nElements, void *p) {
      return p ? new(p) ::DHit_b1_4[nElements] : new ::DHit_b1_4[nElements];
   }
   // Wrapper around operator delete
   static void delete_DHit_b1_4(void *p) {
      delete ((::DHit_b1_4*)p);
   }
   static void deleteArray_DHit_b1_4(void *p) {
      delete [] ((::DHit_b1_4*)p);
   }
   static void destruct_DHit_b1_4(void *p) {
      typedef ::DHit_b1_4 current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::DHit_b1_4

//______________________________________________________________________________
void DParticle_b1_5::Streamer(TBuffer &R__b)
{
   // Stream an object of class DParticle_b1_5.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(DParticle_b1_5::Class(),this);
   } else {
      R__b.WriteClassBuffer(DParticle_b1_5::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_DParticle_b1_5(void *p) {
      return  p ? new(p) ::DParticle_b1_5 : new ::DParticle_b1_5;
   }
   static void *newArray_DParticle_b1_5(Long_t nElements, void *p) {
      return p ? new(p) ::DParticle_b1_5[nElements] : new ::DParticle_b1_5[nElements];
   }
   // Wrapper around operator delete
   static void delete_DParticle_b1_5(void *p) {
      delete ((::DParticle_b1_5*)p);
   }
   static void deleteArray_DParticle_b1_5(void *p) {
      delete [] ((::DParticle_b1_5*)p);
   }
   static void destruct_DParticle_b1_5(void *p) {
      typedef ::DParticle_b1_5 current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::DParticle_b1_5

//______________________________________________________________________________
void DStep_b1_5::Streamer(TBuffer &R__b)
{
   // Stream an object of class DStep_b1_5.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(DStep_b1_5::Class(),this);
   } else {
      R__b.WriteClassBuffer(DStep_b1_5::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_DStep_b1_5(void *p) {
      return  p ? new(p) ::DStep_b1_5 : new ::DStep_b1_5;
   }
   static void *newArray_DStep_b1_5(Long_t nElements, void *p) {
      return p ? new(p) ::DStep_b1_5[nElements] : new ::DStep_b1_5[nElements];
   }
   // Wrapper around operator delete
   static void delete_DStep_b1_5(void *p) {
      delete ((::DStep_b1_5*)p);
   }
   static void deleteArray_DStep_b1_5(void *p) {
      delete [] ((::DStep_b1_5*)p);
   }
   static void destruct_DStep_b1_5(void *p) {
      typedef ::DStep_b1_5 current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::DStep_b1_5

//______________________________________________________________________________
void CalorimeterHit_b1::Streamer(TBuffer &R__b)
{
   // Stream an object of class CalorimeterHit_b1.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(CalorimeterHit_b1::Class(),this);
   } else {
      R__b.WriteClassBuffer(CalorimeterHit_b1::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_CalorimeterHit_b1(void *p) {
      return  p ? new(p) ::CalorimeterHit_b1 : new ::CalorimeterHit_b1;
   }
   static void *newArray_CalorimeterHit_b1(Long_t nElements, void *p) {
      return p ? new(p) ::CalorimeterHit_b1[nElements] : new ::CalorimeterHit_b1[nElements];
   }
   // Wrapper around operator delete
   static void delete_CalorimeterHit_b1(void *p) {
      delete ((::CalorimeterHit_b1*)p);
   }
   static void deleteArray_CalorimeterHit_b1(void *p) {
      delete [] ((::CalorimeterHit_b1*)p);
   }
   static void destruct_CalorimeterHit_b1(void *p) {
      typedef ::CalorimeterHit_b1 current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::CalorimeterHit_b1

//______________________________________________________________________________
void ReconstructedParticle_b1_5::Streamer(TBuffer &R__b)
{
   // Stream an object of class ReconstructedParticle_b1_5.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(ReconstructedParticle_b1_5::Class(),this);
   } else {
      R__b.WriteClassBuffer(ReconstructedParticle_b1_5::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_ReconstructedParticle_b1_5(void *p) {
      return  p ? new(p) ::ReconstructedParticle_b1_5 : new ::ReconstructedParticle_b1_5;
   }
   static void *newArray_ReconstructedParticle_b1_5(Long_t nElements, void *p) {
      return p ? new(p) ::ReconstructedParticle_b1_5[nElements] : new ::ReconstructedParticle_b1_5[nElements];
   }
   // Wrapper around operator delete
   static void delete_ReconstructedParticle_b1_5(void *p) {
      delete ((::ReconstructedParticle_b1_5*)p);
   }
   static void deleteArray_ReconstructedParticle_b1_5(void *p) {
      delete [] ((::ReconstructedParticle_b1_5*)p);
   }
   static void destruct_ReconstructedParticle_b1_5(void *p) {
      typedef ::ReconstructedParticle_b1_5 current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ReconstructedParticle_b1_5

//______________________________________________________________________________
void McParticle_b1_5::Streamer(TBuffer &R__b)
{
   // Stream an object of class McParticle_b1_5.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(McParticle_b1_5::Class(),this);
   } else {
      R__b.WriteClassBuffer(McParticle_b1_5::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_McParticle_b1_5(void *p) {
      return  p ? new(p) ::McParticle_b1_5 : new ::McParticle_b1_5;
   }
   static void *newArray_McParticle_b1_5(Long_t nElements, void *p) {
      return p ? new(p) ::McParticle_b1_5[nElements] : new ::McParticle_b1_5[nElements];
   }
   // Wrapper around operator delete
   static void delete_McParticle_b1_5(void *p) {
      delete ((::McParticle_b1_5*)p);
   }
   static void deleteArray_McParticle_b1_5(void *p) {
      delete [] ((::McParticle_b1_5*)p);
   }
   static void destruct_McParticle_b1_5(void *p) {
      typedef ::McParticle_b1_5 current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::McParticle_b1_5

//______________________________________________________________________________
void SimulatedHit_b1_5::Streamer(TBuffer &R__b)
{
   // Stream an object of class SimulatedHit_b1_5.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(SimulatedHit_b1_5::Class(),this);
   } else {
      R__b.WriteClassBuffer(SimulatedHit_b1_5::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_SimulatedHit_b1_5(void *p) {
      return  p ? new(p) ::SimulatedHit_b1_5 : new ::SimulatedHit_b1_5;
   }
   static void *newArray_SimulatedHit_b1_5(Long_t nElements, void *p) {
      return p ? new(p) ::SimulatedHit_b1_5[nElements] : new ::SimulatedHit_b1_5[nElements];
   }
   // Wrapper around operator delete
   static void delete_SimulatedHit_b1_5(void *p) {
      delete ((::SimulatedHit_b1_5*)p);
   }
   static void deleteArray_SimulatedHit_b1_5(void *p) {
      delete [] ((::SimulatedHit_b1_5*)p);
   }
   static void destruct_SimulatedHit_b1_5(void *p) {
      typedef ::SimulatedHit_b1_5 current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::SimulatedHit_b1_5

//______________________________________________________________________________
void DigiForm_b1::Streamer(TBuffer &R__b)
{
   // Stream an object of class DigiForm_b1.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(DigiForm_b1::Class(),this);
   } else {
      R__b.WriteClassBuffer(DigiForm_b1::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_DigiForm_b1(void *p) {
      return  p ? new(p) ::DigiForm_b1 : new ::DigiForm_b1;
   }
   static void *newArray_DigiForm_b1(Long_t nElements, void *p) {
      return p ? new(p) ::DigiForm_b1[nElements] : new ::DigiForm_b1[nElements];
   }
   // Wrapper around operator delete
   static void delete_DigiForm_b1(void *p) {
      delete ((::DigiForm_b1*)p);
   }
   static void deleteArray_DigiForm_b1(void *p) {
      delete [] ((::DigiForm_b1*)p);
   }
   static void destruct_DigiForm_b1(void *p) {
      typedef ::DigiForm_b1 current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::DigiForm_b1

//______________________________________________________________________________
void McPHelper_b1_5::Streamer(TBuffer &R__b)
{
   // Stream an object of class McPHelper_b1_5.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(McPHelper_b1_5::Class(),this);
   } else {
      R__b.WriteClassBuffer(McPHelper_b1_5::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_McPHelper_b1_5(void *p) {
      return  p ? new(p) ::McPHelper_b1_5 : new ::McPHelper_b1_5;
   }
   static void *newArray_McPHelper_b1_5(Long_t nElements, void *p) {
      return p ? new(p) ::McPHelper_b1_5[nElements] : new ::McPHelper_b1_5[nElements];
   }
   // Wrapper around operator delete
   static void delete_McPHelper_b1_5(void *p) {
      delete ((::McPHelper_b1_5*)p);
   }
   static void deleteArray_McPHelper_b1_5(void *p) {
      delete [] ((::McPHelper_b1_5*)p);
   }
   static void destruct_McPHelper_b1_5(void *p) {
      typedef ::McPHelper_b1_5 current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::McPHelper_b1_5

namespace ROOT {
   // Wrappers around operator new
   static void *new_PhysicsDef(void *p) {
      return  p ? new(p) ::PhysicsDef : new ::PhysicsDef;
   }
   static void *newArray_PhysicsDef(Long_t nElements, void *p) {
      return p ? new(p) ::PhysicsDef[nElements] : new ::PhysicsDef[nElements];
   }
   // Wrapper around operator delete
   static void delete_PhysicsDef(void *p) {
      delete ((::PhysicsDef*)p);
   }
   static void deleteArray_PhysicsDef(void *p) {
      delete [] ((::PhysicsDef*)p);
   }
   static void destruct_PhysicsDef(void *p) {
      typedef ::PhysicsDef current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::PhysicsDef

//______________________________________________________________________________
void DTruthParticle::Streamer(TBuffer &R__b)
{
   // Stream an object of class DTruthParticle.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(DTruthParticle::Class(),this);
   } else {
      R__b.WriteClassBuffer(DTruthParticle::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_DTruthParticle(void *p) {
      return  p ? new(p) ::DTruthParticle : new ::DTruthParticle;
   }
   static void *newArray_DTruthParticle(Long_t nElements, void *p) {
      return p ? new(p) ::DTruthParticle[nElements] : new ::DTruthParticle[nElements];
   }
   // Wrapper around operator delete
   static void delete_DTruthParticle(void *p) {
      delete ((::DTruthParticle*)p);
   }
   static void deleteArray_DTruthParticle(void *p) {
      delete [] ((::DTruthParticle*)p);
   }
   static void destruct_DTruthParticle(void *p) {
      typedef ::DTruthParticle current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::DTruthParticle

//______________________________________________________________________________
void DTruthProcess::Streamer(TBuffer &R__b)
{
   // Stream an object of class DTruthProcess.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(DTruthProcess::Class(),this);
   } else {
      R__b.WriteClassBuffer(DTruthProcess::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_DTruthProcess(void *p) {
      return  p ? new(p) ::DTruthProcess : new ::DTruthProcess;
   }
   static void *newArray_DTruthProcess(Long_t nElements, void *p) {
      return p ? new(p) ::DTruthProcess[nElements] : new ::DTruthProcess[nElements];
   }
   // Wrapper around operator delete
   static void delete_DTruthProcess(void *p) {
      delete ((::DTruthProcess*)p);
   }
   static void deleteArray_DTruthProcess(void *p) {
      delete [] ((::DTruthProcess*)p);
   }
   static void destruct_DTruthProcess(void *p) {
      typedef ::DTruthProcess current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::DTruthProcess

//______________________________________________________________________________
void DTruthState::Streamer(TBuffer &R__b)
{
   // Stream an object of class DTruthState.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(DTruthState::Class(),this);
   } else {
      R__b.WriteClassBuffer(DTruthState::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_DTruthState(void *p) {
      return  p ? new(p) ::DTruthState : new ::DTruthState;
   }
   static void *newArray_DTruthState(Long_t nElements, void *p) {
      return p ? new(p) ::DTruthState[nElements] : new ::DTruthState[nElements];
   }
   // Wrapper around operator delete
   static void delete_DTruthState(void *p) {
      delete ((::DTruthState*)p);
   }
   static void deleteArray_DTruthState(void *p) {
      delete [] ((::DTruthState*)p);
   }
   static void destruct_DTruthState(void *p) {
      typedef ::DTruthState current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::DTruthState

//______________________________________________________________________________
void DTruth::Streamer(TBuffer &R__b)
{
   // Stream an object of class DTruth.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(DTruth::Class(),this);
   } else {
      R__b.WriteClassBuffer(DTruth::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_DTruth(void *p) {
      return  p ? new(p) ::DTruth : new ::DTruth;
   }
   static void *newArray_DTruth(Long_t nElements, void *p) {
      return p ? new(p) ::DTruth[nElements] : new ::DTruth[nElements];
   }
   // Wrapper around operator delete
   static void delete_DTruth(void *p) {
      delete ((::DTruth*)p);
   }
   static void deleteArray_DTruth(void *p) {
      delete [] ((::DTruth*)p);
   }
   static void destruct_DTruth(void *p) {
      typedef ::DTruth current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::DTruth

//______________________________________________________________________________
void DEvent_b1_5::Streamer(TBuffer &R__b)
{
   // Stream an object of class DEvent_b1_5.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(DEvent_b1_5::Class(),this);
   } else {
      R__b.WriteClassBuffer(DEvent_b1_5::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_DEvent_b1_5(void *p) {
      return  p ? new(p) ::DEvent_b1_5 : new ::DEvent_b1_5;
   }
   static void *newArray_DEvent_b1_5(Long_t nElements, void *p) {
      return p ? new(p) ::DEvent_b1_5[nElements] : new ::DEvent_b1_5[nElements];
   }
   // Wrapper around operator delete
   static void delete_DEvent_b1_5(void *p) {
      delete ((::DEvent_b1_5*)p);
   }
   static void deleteArray_DEvent_b1_5(void *p) {
      delete [] ((::DEvent_b1_5*)p);
   }
   static void destruct_DEvent_b1_5(void *p) {
      typedef ::DEvent_b1_5 current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::DEvent_b1_5

//______________________________________________________________________________
void DMagnet_b1::Streamer(TBuffer &R__b)
{
   // Stream an object of class DMagnet_b1.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(DMagnet_b1::Class(),this);
   } else {
      R__b.WriteClassBuffer(DMagnet_b1::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_DMagnet_b1(void *p) {
      return  p ? new(p) ::DMagnet_b1 : new ::DMagnet_b1;
   }
   static void *newArray_DMagnet_b1(Long_t nElements, void *p) {
      return p ? new(p) ::DMagnet_b1[nElements] : new ::DMagnet_b1[nElements];
   }
   // Wrapper around operator delete
   static void delete_DMagnet_b1(void *p) {
      delete ((::DMagnet_b1*)p);
   }
   static void deleteArray_DMagnet_b1(void *p) {
      delete [] ((::DMagnet_b1*)p);
   }
   static void destruct_DMagnet_b1(void *p) {
      typedef ::DMagnet_b1 current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::DMagnet_b1

namespace ROOT {
   // Wrappers around operator new
   static void *new_DMagnet_b1cLcLMagSlice(void *p) {
      return  p ? new(p) ::DMagnet_b1::MagSlice : new ::DMagnet_b1::MagSlice;
   }
   static void *newArray_DMagnet_b1cLcLMagSlice(Long_t nElements, void *p) {
      return p ? new(p) ::DMagnet_b1::MagSlice[nElements] : new ::DMagnet_b1::MagSlice[nElements];
   }
   // Wrapper around operator delete
   static void delete_DMagnet_b1cLcLMagSlice(void *p) {
      delete ((::DMagnet_b1::MagSlice*)p);
   }
   static void deleteArray_DMagnet_b1cLcLMagSlice(void *p) {
      delete [] ((::DMagnet_b1::MagSlice*)p);
   }
   static void destruct_DMagnet_b1cLcLMagSlice(void *p) {
      typedef ::DMagnet_b1::MagSlice current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::DMagnet_b1::MagSlice

//______________________________________________________________________________
void DDetectorIDMaps_b1_5::Streamer(TBuffer &R__b)
{
   // Stream an object of class DDetectorIDMaps_b1_5.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(DDetectorIDMaps_b1_5::Class(),this);
   } else {
      R__b.WriteClassBuffer(DDetectorIDMaps_b1_5::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_DDetectorIDMaps_b1_5(void *p) {
      delete ((::DDetectorIDMaps_b1_5*)p);
   }
   static void deleteArray_DDetectorIDMaps_b1_5(void *p) {
      delete [] ((::DDetectorIDMaps_b1_5*)p);
   }
   static void destruct_DDetectorIDMaps_b1_5(void *p) {
      typedef ::DDetectorIDMaps_b1_5 current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::DDetectorIDMaps_b1_5

namespace ROOT {
   static TClass *vectorlEstringgR_Dictionary();
   static void vectorlEstringgR_TClassManip(TClass*);
   static void *new_vectorlEstringgR(void *p = nullptr);
   static void *newArray_vectorlEstringgR(Long_t size, void *p);
   static void delete_vectorlEstringgR(void *p);
   static void deleteArray_vectorlEstringgR(void *p);
   static void destruct_vectorlEstringgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<string>*)
   {
      vector<string> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<string>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<string>", -2, "vector", 389,
                  typeid(vector<string>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEstringgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<string>) );
      instance.SetNew(&new_vectorlEstringgR);
      instance.SetNewArray(&newArray_vectorlEstringgR);
      instance.SetDelete(&delete_vectorlEstringgR);
      instance.SetDeleteArray(&deleteArray_vectorlEstringgR);
      instance.SetDestructor(&destruct_vectorlEstringgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<string> >()));

      ::ROOT::AddClassAlternate("vector<string>","std::vector<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::allocator<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<string>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEstringgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<string>*)nullptr)->GetClass();
      vectorlEstringgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEstringgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEstringgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<string> : new vector<string>;
   }
   static void *newArray_vectorlEstringgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<string>[nElements] : new vector<string>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEstringgR(void *p) {
      delete ((vector<string>*)p);
   }
   static void deleteArray_vectorlEstringgR(void *p) {
      delete [] ((vector<string>*)p);
   }
   static void destruct_vectorlEstringgR(void *p) {
      typedef vector<string> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<string>

namespace ROOT {
   static TClass *vectorlEintgR_Dictionary();
   static void vectorlEintgR_TClassManip(TClass*);
   static void *new_vectorlEintgR(void *p = nullptr);
   static void *newArray_vectorlEintgR(Long_t size, void *p);
   static void delete_vectorlEintgR(void *p);
   static void deleteArray_vectorlEintgR(void *p);
   static void destruct_vectorlEintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<int>*)
   {
      vector<int> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<int>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<int>", -2, "vector", 389,
                  typeid(vector<int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEintgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<int>) );
      instance.SetNew(&new_vectorlEintgR);
      instance.SetNewArray(&newArray_vectorlEintgR);
      instance.SetDelete(&delete_vectorlEintgR);
      instance.SetDeleteArray(&deleteArray_vectorlEintgR);
      instance.SetDestructor(&destruct_vectorlEintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<int> >()));

      ::ROOT::AddClassAlternate("vector<int>","std::vector<int, std::allocator<int> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<int>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<int>*)nullptr)->GetClass();
      vectorlEintgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEintgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<int> : new vector<int>;
   }
   static void *newArray_vectorlEintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<int>[nElements] : new vector<int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEintgR(void *p) {
      delete ((vector<int>*)p);
   }
   static void deleteArray_vectorlEintgR(void *p) {
      delete [] ((vector<int>*)p);
   }
   static void destruct_vectorlEintgR(void *p) {
      typedef vector<int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<int>

namespace ROOT {
   static TClass *vectorlEfloatgR_Dictionary();
   static void vectorlEfloatgR_TClassManip(TClass*);
   static void *new_vectorlEfloatgR(void *p = nullptr);
   static void *newArray_vectorlEfloatgR(Long_t size, void *p);
   static void delete_vectorlEfloatgR(void *p);
   static void deleteArray_vectorlEfloatgR(void *p);
   static void destruct_vectorlEfloatgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<float>*)
   {
      vector<float> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<float>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<float>", -2, "vector", 389,
                  typeid(vector<float>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEfloatgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<float>) );
      instance.SetNew(&new_vectorlEfloatgR);
      instance.SetNewArray(&newArray_vectorlEfloatgR);
      instance.SetDelete(&delete_vectorlEfloatgR);
      instance.SetDeleteArray(&deleteArray_vectorlEfloatgR);
      instance.SetDestructor(&destruct_vectorlEfloatgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<float> >()));

      ::ROOT::AddClassAlternate("vector<float>","std::vector<float, std::allocator<float> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<float>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEfloatgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<float>*)nullptr)->GetClass();
      vectorlEfloatgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEfloatgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEfloatgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<float> : new vector<float>;
   }
   static void *newArray_vectorlEfloatgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<float>[nElements] : new vector<float>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEfloatgR(void *p) {
      delete ((vector<float>*)p);
   }
   static void deleteArray_vectorlEfloatgR(void *p) {
      delete [] ((vector<float>*)p);
   }
   static void destruct_vectorlEfloatgR(void *p) {
      typedef vector<float> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<float>

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

namespace ROOT {
   static TClass *vectorlETVector3gR_Dictionary();
   static void vectorlETVector3gR_TClassManip(TClass*);
   static void *new_vectorlETVector3gR(void *p = nullptr);
   static void *newArray_vectorlETVector3gR(Long_t size, void *p);
   static void delete_vectorlETVector3gR(void *p);
   static void deleteArray_vectorlETVector3gR(void *p);
   static void destruct_vectorlETVector3gR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<TVector3>*)
   {
      vector<TVector3> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<TVector3>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<TVector3>", -2, "vector", 389,
                  typeid(vector<TVector3>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlETVector3gR_Dictionary, isa_proxy, 4,
                  sizeof(vector<TVector3>) );
      instance.SetNew(&new_vectorlETVector3gR);
      instance.SetNewArray(&newArray_vectorlETVector3gR);
      instance.SetDelete(&delete_vectorlETVector3gR);
      instance.SetDeleteArray(&deleteArray_vectorlETVector3gR);
      instance.SetDestructor(&destruct_vectorlETVector3gR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<TVector3> >()));

      ::ROOT::AddClassAlternate("vector<TVector3>","std::vector<TVector3, std::allocator<TVector3> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<TVector3>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlETVector3gR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<TVector3>*)nullptr)->GetClass();
      vectorlETVector3gR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlETVector3gR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlETVector3gR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TVector3> : new vector<TVector3>;
   }
   static void *newArray_vectorlETVector3gR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TVector3>[nElements] : new vector<TVector3>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlETVector3gR(void *p) {
      delete ((vector<TVector3>*)p);
   }
   static void deleteArray_vectorlETVector3gR(void *p) {
      delete [] ((vector<TVector3>*)p);
   }
   static void destruct_vectorlETVector3gR(void *p) {
      typedef vector<TVector3> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<TVector3>

namespace ROOT {
   static TClass *vectorlESimulatedHit_b1_5mUgR_Dictionary();
   static void vectorlESimulatedHit_b1_5mUgR_TClassManip(TClass*);
   static void *new_vectorlESimulatedHit_b1_5mUgR(void *p = nullptr);
   static void *newArray_vectorlESimulatedHit_b1_5mUgR(Long_t size, void *p);
   static void delete_vectorlESimulatedHit_b1_5mUgR(void *p);
   static void deleteArray_vectorlESimulatedHit_b1_5mUgR(void *p);
   static void destruct_vectorlESimulatedHit_b1_5mUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<SimulatedHit_b1_5*>*)
   {
      vector<SimulatedHit_b1_5*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<SimulatedHit_b1_5*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<SimulatedHit_b1_5*>", -2, "vector", 389,
                  typeid(vector<SimulatedHit_b1_5*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlESimulatedHit_b1_5mUgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<SimulatedHit_b1_5*>) );
      instance.SetNew(&new_vectorlESimulatedHit_b1_5mUgR);
      instance.SetNewArray(&newArray_vectorlESimulatedHit_b1_5mUgR);
      instance.SetDelete(&delete_vectorlESimulatedHit_b1_5mUgR);
      instance.SetDeleteArray(&deleteArray_vectorlESimulatedHit_b1_5mUgR);
      instance.SetDestructor(&destruct_vectorlESimulatedHit_b1_5mUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<SimulatedHit_b1_5*> >()));

      ::ROOT::AddClassAlternate("vector<SimulatedHit_b1_5*>","std::vector<SimulatedHit_b1_5*, std::allocator<SimulatedHit_b1_5*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<SimulatedHit_b1_5*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlESimulatedHit_b1_5mUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<SimulatedHit_b1_5*>*)nullptr)->GetClass();
      vectorlESimulatedHit_b1_5mUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlESimulatedHit_b1_5mUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlESimulatedHit_b1_5mUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<SimulatedHit_b1_5*> : new vector<SimulatedHit_b1_5*>;
   }
   static void *newArray_vectorlESimulatedHit_b1_5mUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<SimulatedHit_b1_5*>[nElements] : new vector<SimulatedHit_b1_5*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlESimulatedHit_b1_5mUgR(void *p) {
      delete ((vector<SimulatedHit_b1_5*>*)p);
   }
   static void deleteArray_vectorlESimulatedHit_b1_5mUgR(void *p) {
      delete [] ((vector<SimulatedHit_b1_5*>*)p);
   }
   static void destruct_vectorlESimulatedHit_b1_5mUgR(void *p) {
      typedef vector<SimulatedHit_b1_5*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<SimulatedHit_b1_5*>

namespace ROOT {
   static TClass *vectorlEReconstructedParticle_b1_5mUgR_Dictionary();
   static void vectorlEReconstructedParticle_b1_5mUgR_TClassManip(TClass*);
   static void *new_vectorlEReconstructedParticle_b1_5mUgR(void *p = nullptr);
   static void *newArray_vectorlEReconstructedParticle_b1_5mUgR(Long_t size, void *p);
   static void delete_vectorlEReconstructedParticle_b1_5mUgR(void *p);
   static void deleteArray_vectorlEReconstructedParticle_b1_5mUgR(void *p);
   static void destruct_vectorlEReconstructedParticle_b1_5mUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<ReconstructedParticle_b1_5*>*)
   {
      vector<ReconstructedParticle_b1_5*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<ReconstructedParticle_b1_5*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<ReconstructedParticle_b1_5*>", -2, "vector", 389,
                  typeid(vector<ReconstructedParticle_b1_5*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEReconstructedParticle_b1_5mUgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<ReconstructedParticle_b1_5*>) );
      instance.SetNew(&new_vectorlEReconstructedParticle_b1_5mUgR);
      instance.SetNewArray(&newArray_vectorlEReconstructedParticle_b1_5mUgR);
      instance.SetDelete(&delete_vectorlEReconstructedParticle_b1_5mUgR);
      instance.SetDeleteArray(&deleteArray_vectorlEReconstructedParticle_b1_5mUgR);
      instance.SetDestructor(&destruct_vectorlEReconstructedParticle_b1_5mUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<ReconstructedParticle_b1_5*> >()));

      ::ROOT::AddClassAlternate("vector<ReconstructedParticle_b1_5*>","std::vector<ReconstructedParticle_b1_5*, std::allocator<ReconstructedParticle_b1_5*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<ReconstructedParticle_b1_5*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEReconstructedParticle_b1_5mUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<ReconstructedParticle_b1_5*>*)nullptr)->GetClass();
      vectorlEReconstructedParticle_b1_5mUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEReconstructedParticle_b1_5mUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEReconstructedParticle_b1_5mUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<ReconstructedParticle_b1_5*> : new vector<ReconstructedParticle_b1_5*>;
   }
   static void *newArray_vectorlEReconstructedParticle_b1_5mUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<ReconstructedParticle_b1_5*>[nElements] : new vector<ReconstructedParticle_b1_5*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEReconstructedParticle_b1_5mUgR(void *p) {
      delete ((vector<ReconstructedParticle_b1_5*>*)p);
   }
   static void deleteArray_vectorlEReconstructedParticle_b1_5mUgR(void *p) {
      delete [] ((vector<ReconstructedParticle_b1_5*>*)p);
   }
   static void destruct_vectorlEReconstructedParticle_b1_5mUgR(void *p) {
      typedef vector<ReconstructedParticle_b1_5*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<ReconstructedParticle_b1_5*>

namespace ROOT {
   static TClass *vectorlEMcParticle_b1_5gR_Dictionary();
   static void vectorlEMcParticle_b1_5gR_TClassManip(TClass*);
   static void *new_vectorlEMcParticle_b1_5gR(void *p = nullptr);
   static void *newArray_vectorlEMcParticle_b1_5gR(Long_t size, void *p);
   static void delete_vectorlEMcParticle_b1_5gR(void *p);
   static void deleteArray_vectorlEMcParticle_b1_5gR(void *p);
   static void destruct_vectorlEMcParticle_b1_5gR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<McParticle_b1_5>*)
   {
      vector<McParticle_b1_5> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<McParticle_b1_5>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<McParticle_b1_5>", -2, "vector", 389,
                  typeid(vector<McParticle_b1_5>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEMcParticle_b1_5gR_Dictionary, isa_proxy, 0,
                  sizeof(vector<McParticle_b1_5>) );
      instance.SetNew(&new_vectorlEMcParticle_b1_5gR);
      instance.SetNewArray(&newArray_vectorlEMcParticle_b1_5gR);
      instance.SetDelete(&delete_vectorlEMcParticle_b1_5gR);
      instance.SetDeleteArray(&deleteArray_vectorlEMcParticle_b1_5gR);
      instance.SetDestructor(&destruct_vectorlEMcParticle_b1_5gR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<McParticle_b1_5> >()));

      ::ROOT::AddClassAlternate("vector<McParticle_b1_5>","std::vector<McParticle_b1_5, std::allocator<McParticle_b1_5> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<McParticle_b1_5>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEMcParticle_b1_5gR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<McParticle_b1_5>*)nullptr)->GetClass();
      vectorlEMcParticle_b1_5gR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEMcParticle_b1_5gR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEMcParticle_b1_5gR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<McParticle_b1_5> : new vector<McParticle_b1_5>;
   }
   static void *newArray_vectorlEMcParticle_b1_5gR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<McParticle_b1_5>[nElements] : new vector<McParticle_b1_5>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEMcParticle_b1_5gR(void *p) {
      delete ((vector<McParticle_b1_5>*)p);
   }
   static void deleteArray_vectorlEMcParticle_b1_5gR(void *p) {
      delete [] ((vector<McParticle_b1_5>*)p);
   }
   static void destruct_vectorlEMcParticle_b1_5gR(void *p) {
      typedef vector<McParticle_b1_5> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<McParticle_b1_5>

namespace ROOT {
   static TClass *vectorlEMcParticle_b1_5mUgR_Dictionary();
   static void vectorlEMcParticle_b1_5mUgR_TClassManip(TClass*);
   static void *new_vectorlEMcParticle_b1_5mUgR(void *p = nullptr);
   static void *newArray_vectorlEMcParticle_b1_5mUgR(Long_t size, void *p);
   static void delete_vectorlEMcParticle_b1_5mUgR(void *p);
   static void deleteArray_vectorlEMcParticle_b1_5mUgR(void *p);
   static void destruct_vectorlEMcParticle_b1_5mUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<McParticle_b1_5*>*)
   {
      vector<McParticle_b1_5*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<McParticle_b1_5*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<McParticle_b1_5*>", -2, "vector", 389,
                  typeid(vector<McParticle_b1_5*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEMcParticle_b1_5mUgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<McParticle_b1_5*>) );
      instance.SetNew(&new_vectorlEMcParticle_b1_5mUgR);
      instance.SetNewArray(&newArray_vectorlEMcParticle_b1_5mUgR);
      instance.SetDelete(&delete_vectorlEMcParticle_b1_5mUgR);
      instance.SetDeleteArray(&deleteArray_vectorlEMcParticle_b1_5mUgR);
      instance.SetDestructor(&destruct_vectorlEMcParticle_b1_5mUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<McParticle_b1_5*> >()));

      ::ROOT::AddClassAlternate("vector<McParticle_b1_5*>","std::vector<McParticle_b1_5*, std::allocator<McParticle_b1_5*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<McParticle_b1_5*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEMcParticle_b1_5mUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<McParticle_b1_5*>*)nullptr)->GetClass();
      vectorlEMcParticle_b1_5mUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEMcParticle_b1_5mUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEMcParticle_b1_5mUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<McParticle_b1_5*> : new vector<McParticle_b1_5*>;
   }
   static void *newArray_vectorlEMcParticle_b1_5mUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<McParticle_b1_5*>[nElements] : new vector<McParticle_b1_5*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEMcParticle_b1_5mUgR(void *p) {
      delete ((vector<McParticle_b1_5*>*)p);
   }
   static void deleteArray_vectorlEMcParticle_b1_5mUgR(void *p) {
      delete [] ((vector<McParticle_b1_5*>*)p);
   }
   static void destruct_vectorlEMcParticle_b1_5mUgR(void *p) {
      typedef vector<McParticle_b1_5*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<McParticle_b1_5*>

namespace ROOT {
   static TClass *vectorlEMcPHelper_b1_5mUgR_Dictionary();
   static void vectorlEMcPHelper_b1_5mUgR_TClassManip(TClass*);
   static void *new_vectorlEMcPHelper_b1_5mUgR(void *p = nullptr);
   static void *newArray_vectorlEMcPHelper_b1_5mUgR(Long_t size, void *p);
   static void delete_vectorlEMcPHelper_b1_5mUgR(void *p);
   static void deleteArray_vectorlEMcPHelper_b1_5mUgR(void *p);
   static void destruct_vectorlEMcPHelper_b1_5mUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<McPHelper_b1_5*>*)
   {
      vector<McPHelper_b1_5*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<McPHelper_b1_5*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<McPHelper_b1_5*>", -2, "vector", 389,
                  typeid(vector<McPHelper_b1_5*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEMcPHelper_b1_5mUgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<McPHelper_b1_5*>) );
      instance.SetNew(&new_vectorlEMcPHelper_b1_5mUgR);
      instance.SetNewArray(&newArray_vectorlEMcPHelper_b1_5mUgR);
      instance.SetDelete(&delete_vectorlEMcPHelper_b1_5mUgR);
      instance.SetDeleteArray(&deleteArray_vectorlEMcPHelper_b1_5mUgR);
      instance.SetDestructor(&destruct_vectorlEMcPHelper_b1_5mUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<McPHelper_b1_5*> >()));

      ::ROOT::AddClassAlternate("vector<McPHelper_b1_5*>","std::vector<McPHelper_b1_5*, std::allocator<McPHelper_b1_5*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<McPHelper_b1_5*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEMcPHelper_b1_5mUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<McPHelper_b1_5*>*)nullptr)->GetClass();
      vectorlEMcPHelper_b1_5mUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEMcPHelper_b1_5mUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEMcPHelper_b1_5mUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<McPHelper_b1_5*> : new vector<McPHelper_b1_5*>;
   }
   static void *newArray_vectorlEMcPHelper_b1_5mUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<McPHelper_b1_5*>[nElements] : new vector<McPHelper_b1_5*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEMcPHelper_b1_5mUgR(void *p) {
      delete ((vector<McPHelper_b1_5*>*)p);
   }
   static void deleteArray_vectorlEMcPHelper_b1_5mUgR(void *p) {
      delete [] ((vector<McPHelper_b1_5*>*)p);
   }
   static void destruct_vectorlEMcPHelper_b1_5mUgR(void *p) {
      typedef vector<McPHelper_b1_5*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<McPHelper_b1_5*>

namespace ROOT {
   static TClass *vectorlEDigiForm_b1mUgR_Dictionary();
   static void vectorlEDigiForm_b1mUgR_TClassManip(TClass*);
   static void *new_vectorlEDigiForm_b1mUgR(void *p = nullptr);
   static void *newArray_vectorlEDigiForm_b1mUgR(Long_t size, void *p);
   static void delete_vectorlEDigiForm_b1mUgR(void *p);
   static void deleteArray_vectorlEDigiForm_b1mUgR(void *p);
   static void destruct_vectorlEDigiForm_b1mUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<DigiForm_b1*>*)
   {
      vector<DigiForm_b1*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<DigiForm_b1*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<DigiForm_b1*>", -2, "vector", 389,
                  typeid(vector<DigiForm_b1*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEDigiForm_b1mUgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<DigiForm_b1*>) );
      instance.SetNew(&new_vectorlEDigiForm_b1mUgR);
      instance.SetNewArray(&newArray_vectorlEDigiForm_b1mUgR);
      instance.SetDelete(&delete_vectorlEDigiForm_b1mUgR);
      instance.SetDeleteArray(&deleteArray_vectorlEDigiForm_b1mUgR);
      instance.SetDestructor(&destruct_vectorlEDigiForm_b1mUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<DigiForm_b1*> >()));

      ::ROOT::AddClassAlternate("vector<DigiForm_b1*>","std::vector<DigiForm_b1*, std::allocator<DigiForm_b1*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<DigiForm_b1*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEDigiForm_b1mUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<DigiForm_b1*>*)nullptr)->GetClass();
      vectorlEDigiForm_b1mUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEDigiForm_b1mUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEDigiForm_b1mUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<DigiForm_b1*> : new vector<DigiForm_b1*>;
   }
   static void *newArray_vectorlEDigiForm_b1mUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<DigiForm_b1*>[nElements] : new vector<DigiForm_b1*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEDigiForm_b1mUgR(void *p) {
      delete ((vector<DigiForm_b1*>*)p);
   }
   static void deleteArray_vectorlEDigiForm_b1mUgR(void *p) {
      delete [] ((vector<DigiForm_b1*>*)p);
   }
   static void destruct_vectorlEDigiForm_b1mUgR(void *p) {
      typedef vector<DigiForm_b1*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<DigiForm_b1*>

namespace ROOT {
   static TClass *vectorlEDTruthStatemUgR_Dictionary();
   static void vectorlEDTruthStatemUgR_TClassManip(TClass*);
   static void *new_vectorlEDTruthStatemUgR(void *p = nullptr);
   static void *newArray_vectorlEDTruthStatemUgR(Long_t size, void *p);
   static void delete_vectorlEDTruthStatemUgR(void *p);
   static void deleteArray_vectorlEDTruthStatemUgR(void *p);
   static void destruct_vectorlEDTruthStatemUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<DTruthState*>*)
   {
      vector<DTruthState*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<DTruthState*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<DTruthState*>", -2, "vector", 389,
                  typeid(vector<DTruthState*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEDTruthStatemUgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<DTruthState*>) );
      instance.SetNew(&new_vectorlEDTruthStatemUgR);
      instance.SetNewArray(&newArray_vectorlEDTruthStatemUgR);
      instance.SetDelete(&delete_vectorlEDTruthStatemUgR);
      instance.SetDeleteArray(&deleteArray_vectorlEDTruthStatemUgR);
      instance.SetDestructor(&destruct_vectorlEDTruthStatemUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<DTruthState*> >()));

      ::ROOT::AddClassAlternate("vector<DTruthState*>","std::vector<DTruthState*, std::allocator<DTruthState*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<DTruthState*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEDTruthStatemUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<DTruthState*>*)nullptr)->GetClass();
      vectorlEDTruthStatemUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEDTruthStatemUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEDTruthStatemUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<DTruthState*> : new vector<DTruthState*>;
   }
   static void *newArray_vectorlEDTruthStatemUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<DTruthState*>[nElements] : new vector<DTruthState*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEDTruthStatemUgR(void *p) {
      delete ((vector<DTruthState*>*)p);
   }
   static void deleteArray_vectorlEDTruthStatemUgR(void *p) {
      delete [] ((vector<DTruthState*>*)p);
   }
   static void destruct_vectorlEDTruthStatemUgR(void *p) {
      typedef vector<DTruthState*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<DTruthState*>

namespace ROOT {
   static TClass *vectorlEDTruthProcessmUgR_Dictionary();
   static void vectorlEDTruthProcessmUgR_TClassManip(TClass*);
   static void *new_vectorlEDTruthProcessmUgR(void *p = nullptr);
   static void *newArray_vectorlEDTruthProcessmUgR(Long_t size, void *p);
   static void delete_vectorlEDTruthProcessmUgR(void *p);
   static void deleteArray_vectorlEDTruthProcessmUgR(void *p);
   static void destruct_vectorlEDTruthProcessmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<DTruthProcess*>*)
   {
      vector<DTruthProcess*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<DTruthProcess*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<DTruthProcess*>", -2, "vector", 389,
                  typeid(vector<DTruthProcess*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEDTruthProcessmUgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<DTruthProcess*>) );
      instance.SetNew(&new_vectorlEDTruthProcessmUgR);
      instance.SetNewArray(&newArray_vectorlEDTruthProcessmUgR);
      instance.SetDelete(&delete_vectorlEDTruthProcessmUgR);
      instance.SetDeleteArray(&deleteArray_vectorlEDTruthProcessmUgR);
      instance.SetDestructor(&destruct_vectorlEDTruthProcessmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<DTruthProcess*> >()));

      ::ROOT::AddClassAlternate("vector<DTruthProcess*>","std::vector<DTruthProcess*, std::allocator<DTruthProcess*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<DTruthProcess*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEDTruthProcessmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<DTruthProcess*>*)nullptr)->GetClass();
      vectorlEDTruthProcessmUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEDTruthProcessmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEDTruthProcessmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<DTruthProcess*> : new vector<DTruthProcess*>;
   }
   static void *newArray_vectorlEDTruthProcessmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<DTruthProcess*>[nElements] : new vector<DTruthProcess*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEDTruthProcessmUgR(void *p) {
      delete ((vector<DTruthProcess*>*)p);
   }
   static void deleteArray_vectorlEDTruthProcessmUgR(void *p) {
      delete [] ((vector<DTruthProcess*>*)p);
   }
   static void destruct_vectorlEDTruthProcessmUgR(void *p) {
      typedef vector<DTruthProcess*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<DTruthProcess*>

namespace ROOT {
   static TClass *vectorlEDTruthParticlemUgR_Dictionary();
   static void vectorlEDTruthParticlemUgR_TClassManip(TClass*);
   static void *new_vectorlEDTruthParticlemUgR(void *p = nullptr);
   static void *newArray_vectorlEDTruthParticlemUgR(Long_t size, void *p);
   static void delete_vectorlEDTruthParticlemUgR(void *p);
   static void deleteArray_vectorlEDTruthParticlemUgR(void *p);
   static void destruct_vectorlEDTruthParticlemUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<DTruthParticle*>*)
   {
      vector<DTruthParticle*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<DTruthParticle*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<DTruthParticle*>", -2, "vector", 389,
                  typeid(vector<DTruthParticle*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEDTruthParticlemUgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<DTruthParticle*>) );
      instance.SetNew(&new_vectorlEDTruthParticlemUgR);
      instance.SetNewArray(&newArray_vectorlEDTruthParticlemUgR);
      instance.SetDelete(&delete_vectorlEDTruthParticlemUgR);
      instance.SetDeleteArray(&deleteArray_vectorlEDTruthParticlemUgR);
      instance.SetDestructor(&destruct_vectorlEDTruthParticlemUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<DTruthParticle*> >()));

      ::ROOT::AddClassAlternate("vector<DTruthParticle*>","std::vector<DTruthParticle*, std::allocator<DTruthParticle*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<DTruthParticle*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEDTruthParticlemUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<DTruthParticle*>*)nullptr)->GetClass();
      vectorlEDTruthParticlemUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEDTruthParticlemUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEDTruthParticlemUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<DTruthParticle*> : new vector<DTruthParticle*>;
   }
   static void *newArray_vectorlEDTruthParticlemUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<DTruthParticle*>[nElements] : new vector<DTruthParticle*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEDTruthParticlemUgR(void *p) {
      delete ((vector<DTruthParticle*>*)p);
   }
   static void deleteArray_vectorlEDTruthParticlemUgR(void *p) {
      delete [] ((vector<DTruthParticle*>*)p);
   }
   static void destruct_vectorlEDTruthParticlemUgR(void *p) {
      typedef vector<DTruthParticle*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<DTruthParticle*>

namespace ROOT {
   static TClass *vectorlEDStep_b1_5mUgR_Dictionary();
   static void vectorlEDStep_b1_5mUgR_TClassManip(TClass*);
   static void *new_vectorlEDStep_b1_5mUgR(void *p = nullptr);
   static void *newArray_vectorlEDStep_b1_5mUgR(Long_t size, void *p);
   static void delete_vectorlEDStep_b1_5mUgR(void *p);
   static void deleteArray_vectorlEDStep_b1_5mUgR(void *p);
   static void destruct_vectorlEDStep_b1_5mUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<DStep_b1_5*>*)
   {
      vector<DStep_b1_5*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<DStep_b1_5*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<DStep_b1_5*>", -2, "vector", 389,
                  typeid(vector<DStep_b1_5*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEDStep_b1_5mUgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<DStep_b1_5*>) );
      instance.SetNew(&new_vectorlEDStep_b1_5mUgR);
      instance.SetNewArray(&newArray_vectorlEDStep_b1_5mUgR);
      instance.SetDelete(&delete_vectorlEDStep_b1_5mUgR);
      instance.SetDeleteArray(&deleteArray_vectorlEDStep_b1_5mUgR);
      instance.SetDestructor(&destruct_vectorlEDStep_b1_5mUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<DStep_b1_5*> >()));

      ::ROOT::AddClassAlternate("vector<DStep_b1_5*>","std::vector<DStep_b1_5*, std::allocator<DStep_b1_5*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<DStep_b1_5*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEDStep_b1_5mUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<DStep_b1_5*>*)nullptr)->GetClass();
      vectorlEDStep_b1_5mUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEDStep_b1_5mUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEDStep_b1_5mUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<DStep_b1_5*> : new vector<DStep_b1_5*>;
   }
   static void *newArray_vectorlEDStep_b1_5mUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<DStep_b1_5*>[nElements] : new vector<DStep_b1_5*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEDStep_b1_5mUgR(void *p) {
      delete ((vector<DStep_b1_5*>*)p);
   }
   static void deleteArray_vectorlEDStep_b1_5mUgR(void *p) {
      delete [] ((vector<DStep_b1_5*>*)p);
   }
   static void destruct_vectorlEDStep_b1_5mUgR(void *p) {
      typedef vector<DStep_b1_5*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<DStep_b1_5*>

namespace ROOT {
   static TClass *vectorlEDParticle_b1_5mUgR_Dictionary();
   static void vectorlEDParticle_b1_5mUgR_TClassManip(TClass*);
   static void *new_vectorlEDParticle_b1_5mUgR(void *p = nullptr);
   static void *newArray_vectorlEDParticle_b1_5mUgR(Long_t size, void *p);
   static void delete_vectorlEDParticle_b1_5mUgR(void *p);
   static void deleteArray_vectorlEDParticle_b1_5mUgR(void *p);
   static void destruct_vectorlEDParticle_b1_5mUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<DParticle_b1_5*>*)
   {
      vector<DParticle_b1_5*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<DParticle_b1_5*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<DParticle_b1_5*>", -2, "vector", 389,
                  typeid(vector<DParticle_b1_5*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEDParticle_b1_5mUgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<DParticle_b1_5*>) );
      instance.SetNew(&new_vectorlEDParticle_b1_5mUgR);
      instance.SetNewArray(&newArray_vectorlEDParticle_b1_5mUgR);
      instance.SetDelete(&delete_vectorlEDParticle_b1_5mUgR);
      instance.SetDeleteArray(&deleteArray_vectorlEDParticle_b1_5mUgR);
      instance.SetDestructor(&destruct_vectorlEDParticle_b1_5mUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<DParticle_b1_5*> >()));

      ::ROOT::AddClassAlternate("vector<DParticle_b1_5*>","std::vector<DParticle_b1_5*, std::allocator<DParticle_b1_5*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<DParticle_b1_5*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEDParticle_b1_5mUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<DParticle_b1_5*>*)nullptr)->GetClass();
      vectorlEDParticle_b1_5mUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEDParticle_b1_5mUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEDParticle_b1_5mUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<DParticle_b1_5*> : new vector<DParticle_b1_5*>;
   }
   static void *newArray_vectorlEDParticle_b1_5mUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<DParticle_b1_5*>[nElements] : new vector<DParticle_b1_5*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEDParticle_b1_5mUgR(void *p) {
      delete ((vector<DParticle_b1_5*>*)p);
   }
   static void deleteArray_vectorlEDParticle_b1_5mUgR(void *p) {
      delete [] ((vector<DParticle_b1_5*>*)p);
   }
   static void destruct_vectorlEDParticle_b1_5mUgR(void *p) {
      typedef vector<DParticle_b1_5*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<DParticle_b1_5*>

namespace ROOT {
   static TClass *vectorlEDMagnet_b1cLcLMagSlicegR_Dictionary();
   static void vectorlEDMagnet_b1cLcLMagSlicegR_TClassManip(TClass*);
   static void *new_vectorlEDMagnet_b1cLcLMagSlicegR(void *p = nullptr);
   static void *newArray_vectorlEDMagnet_b1cLcLMagSlicegR(Long_t size, void *p);
   static void delete_vectorlEDMagnet_b1cLcLMagSlicegR(void *p);
   static void deleteArray_vectorlEDMagnet_b1cLcLMagSlicegR(void *p);
   static void destruct_vectorlEDMagnet_b1cLcLMagSlicegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<DMagnet_b1::MagSlice>*)
   {
      vector<DMagnet_b1::MagSlice> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<DMagnet_b1::MagSlice>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<DMagnet_b1::MagSlice>", -2, "vector", 389,
                  typeid(vector<DMagnet_b1::MagSlice>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEDMagnet_b1cLcLMagSlicegR_Dictionary, isa_proxy, 0,
                  sizeof(vector<DMagnet_b1::MagSlice>) );
      instance.SetNew(&new_vectorlEDMagnet_b1cLcLMagSlicegR);
      instance.SetNewArray(&newArray_vectorlEDMagnet_b1cLcLMagSlicegR);
      instance.SetDelete(&delete_vectorlEDMagnet_b1cLcLMagSlicegR);
      instance.SetDeleteArray(&deleteArray_vectorlEDMagnet_b1cLcLMagSlicegR);
      instance.SetDestructor(&destruct_vectorlEDMagnet_b1cLcLMagSlicegR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<DMagnet_b1::MagSlice> >()));

      ::ROOT::AddClassAlternate("vector<DMagnet_b1::MagSlice>","std::vector<DMagnet_b1::MagSlice, std::allocator<DMagnet_b1::MagSlice> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<DMagnet_b1::MagSlice>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEDMagnet_b1cLcLMagSlicegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<DMagnet_b1::MagSlice>*)nullptr)->GetClass();
      vectorlEDMagnet_b1cLcLMagSlicegR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEDMagnet_b1cLcLMagSlicegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEDMagnet_b1cLcLMagSlicegR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<DMagnet_b1::MagSlice> : new vector<DMagnet_b1::MagSlice>;
   }
   static void *newArray_vectorlEDMagnet_b1cLcLMagSlicegR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<DMagnet_b1::MagSlice>[nElements] : new vector<DMagnet_b1::MagSlice>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEDMagnet_b1cLcLMagSlicegR(void *p) {
      delete ((vector<DMagnet_b1::MagSlice>*)p);
   }
   static void deleteArray_vectorlEDMagnet_b1cLcLMagSlicegR(void *p) {
      delete [] ((vector<DMagnet_b1::MagSlice>*)p);
   }
   static void destruct_vectorlEDMagnet_b1cLcLMagSlicegR(void *p) {
      typedef vector<DMagnet_b1::MagSlice> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<DMagnet_b1::MagSlice>

namespace ROOT {
   static TClass *vectorlEDHit_b1_4mUgR_Dictionary();
   static void vectorlEDHit_b1_4mUgR_TClassManip(TClass*);
   static void *new_vectorlEDHit_b1_4mUgR(void *p = nullptr);
   static void *newArray_vectorlEDHit_b1_4mUgR(Long_t size, void *p);
   static void delete_vectorlEDHit_b1_4mUgR(void *p);
   static void deleteArray_vectorlEDHit_b1_4mUgR(void *p);
   static void destruct_vectorlEDHit_b1_4mUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<DHit_b1_4*>*)
   {
      vector<DHit_b1_4*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<DHit_b1_4*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<DHit_b1_4*>", -2, "vector", 389,
                  typeid(vector<DHit_b1_4*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEDHit_b1_4mUgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<DHit_b1_4*>) );
      instance.SetNew(&new_vectorlEDHit_b1_4mUgR);
      instance.SetNewArray(&newArray_vectorlEDHit_b1_4mUgR);
      instance.SetDelete(&delete_vectorlEDHit_b1_4mUgR);
      instance.SetDeleteArray(&deleteArray_vectorlEDHit_b1_4mUgR);
      instance.SetDestructor(&destruct_vectorlEDHit_b1_4mUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<DHit_b1_4*> >()));

      ::ROOT::AddClassAlternate("vector<DHit_b1_4*>","std::vector<DHit_b1_4*, std::allocator<DHit_b1_4*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<DHit_b1_4*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEDHit_b1_4mUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<DHit_b1_4*>*)nullptr)->GetClass();
      vectorlEDHit_b1_4mUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEDHit_b1_4mUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEDHit_b1_4mUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<DHit_b1_4*> : new vector<DHit_b1_4*>;
   }
   static void *newArray_vectorlEDHit_b1_4mUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<DHit_b1_4*>[nElements] : new vector<DHit_b1_4*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEDHit_b1_4mUgR(void *p) {
      delete ((vector<DHit_b1_4*>*)p);
   }
   static void deleteArray_vectorlEDHit_b1_4mUgR(void *p) {
      delete [] ((vector<DHit_b1_4*>*)p);
   }
   static void destruct_vectorlEDHit_b1_4mUgR(void *p) {
      typedef vector<DHit_b1_4*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<DHit_b1_4*>

namespace ROOT {
   static TClass *vectorlECalorimeterHit_b1mUgR_Dictionary();
   static void vectorlECalorimeterHit_b1mUgR_TClassManip(TClass*);
   static void *new_vectorlECalorimeterHit_b1mUgR(void *p = nullptr);
   static void *newArray_vectorlECalorimeterHit_b1mUgR(Long_t size, void *p);
   static void delete_vectorlECalorimeterHit_b1mUgR(void *p);
   static void deleteArray_vectorlECalorimeterHit_b1mUgR(void *p);
   static void destruct_vectorlECalorimeterHit_b1mUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<CalorimeterHit_b1*>*)
   {
      vector<CalorimeterHit_b1*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<CalorimeterHit_b1*>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<CalorimeterHit_b1*>", -2, "vector", 389,
                  typeid(vector<CalorimeterHit_b1*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlECalorimeterHit_b1mUgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<CalorimeterHit_b1*>) );
      instance.SetNew(&new_vectorlECalorimeterHit_b1mUgR);
      instance.SetNewArray(&newArray_vectorlECalorimeterHit_b1mUgR);
      instance.SetDelete(&delete_vectorlECalorimeterHit_b1mUgR);
      instance.SetDeleteArray(&deleteArray_vectorlECalorimeterHit_b1mUgR);
      instance.SetDestructor(&destruct_vectorlECalorimeterHit_b1mUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<CalorimeterHit_b1*> >()));

      ::ROOT::AddClassAlternate("vector<CalorimeterHit_b1*>","std::vector<CalorimeterHit_b1*, std::allocator<CalorimeterHit_b1*> >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<CalorimeterHit_b1*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlECalorimeterHit_b1mUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<CalorimeterHit_b1*>*)nullptr)->GetClass();
      vectorlECalorimeterHit_b1mUgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlECalorimeterHit_b1mUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlECalorimeterHit_b1mUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CalorimeterHit_b1*> : new vector<CalorimeterHit_b1*>;
   }
   static void *newArray_vectorlECalorimeterHit_b1mUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<CalorimeterHit_b1*>[nElements] : new vector<CalorimeterHit_b1*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlECalorimeterHit_b1mUgR(void *p) {
      delete ((vector<CalorimeterHit_b1*>*)p);
   }
   static void deleteArray_vectorlECalorimeterHit_b1mUgR(void *p) {
      delete [] ((vector<CalorimeterHit_b1*>*)p);
   }
   static void destruct_vectorlECalorimeterHit_b1mUgR(void *p) {
      typedef vector<CalorimeterHit_b1*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<CalorimeterHit_b1*>

namespace ROOT {
   static TClass *unordered_maplEstringcOintgR_Dictionary();
   static void unordered_maplEstringcOintgR_TClassManip(TClass*);
   static void *new_unordered_maplEstringcOintgR(void *p = nullptr);
   static void *newArray_unordered_maplEstringcOintgR(Long_t size, void *p);
   static void delete_unordered_maplEstringcOintgR(void *p);
   static void deleteArray_unordered_maplEstringcOintgR(void *p);
   static void destruct_unordered_maplEstringcOintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const unordered_map<string,int>*)
   {
      unordered_map<string,int> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(unordered_map<string,int>));
      static ::ROOT::TGenericClassInfo 
         instance("unordered_map<string,int>", -2, "unordered_map", 102,
                  typeid(unordered_map<string,int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &unordered_maplEstringcOintgR_Dictionary, isa_proxy, 0,
                  sizeof(unordered_map<string,int>) );
      instance.SetNew(&new_unordered_maplEstringcOintgR);
      instance.SetNewArray(&newArray_unordered_maplEstringcOintgR);
      instance.SetDelete(&delete_unordered_maplEstringcOintgR);
      instance.SetDeleteArray(&deleteArray_unordered_maplEstringcOintgR);
      instance.SetDestructor(&destruct_unordered_maplEstringcOintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< unordered_map<string,int> >()));

      ::ROOT::AddClassAlternate("unordered_map<string,int>","std::unordered_map<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, int, std::hash<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::equal_to<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const, int> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const unordered_map<string,int>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *unordered_maplEstringcOintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const unordered_map<string,int>*)nullptr)->GetClass();
      unordered_maplEstringcOintgR_TClassManip(theClass);
   return theClass;
   }

   static void unordered_maplEstringcOintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_unordered_maplEstringcOintgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) unordered_map<string,int> : new unordered_map<string,int>;
   }
   static void *newArray_unordered_maplEstringcOintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) unordered_map<string,int>[nElements] : new unordered_map<string,int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_unordered_maplEstringcOintgR(void *p) {
      delete ((unordered_map<string,int>*)p);
   }
   static void deleteArray_unordered_maplEstringcOintgR(void *p) {
      delete [] ((unordered_map<string,int>*)p);
   }
   static void destruct_unordered_maplEstringcOintgR(void *p) {
      typedef unordered_map<string,int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class unordered_map<string,int>

namespace ROOT {
   static TClass *maplEstringcOvectorlESimulatedHit_b1_5mUgRmUgR_Dictionary();
   static void maplEstringcOvectorlESimulatedHit_b1_5mUgRmUgR_TClassManip(TClass*);
   static void *new_maplEstringcOvectorlESimulatedHit_b1_5mUgRmUgR(void *p = nullptr);
   static void *newArray_maplEstringcOvectorlESimulatedHit_b1_5mUgRmUgR(Long_t size, void *p);
   static void delete_maplEstringcOvectorlESimulatedHit_b1_5mUgRmUgR(void *p);
   static void deleteArray_maplEstringcOvectorlESimulatedHit_b1_5mUgRmUgR(void *p);
   static void destruct_maplEstringcOvectorlESimulatedHit_b1_5mUgRmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<string,vector<SimulatedHit_b1_5*>*>*)
   {
      map<string,vector<SimulatedHit_b1_5*>*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<string,vector<SimulatedHit_b1_5*>*>));
      static ::ROOT::TGenericClassInfo 
         instance("map<string,vector<SimulatedHit_b1_5*>*>", -2, "map", 100,
                  typeid(map<string,vector<SimulatedHit_b1_5*>*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEstringcOvectorlESimulatedHit_b1_5mUgRmUgR_Dictionary, isa_proxy, 0,
                  sizeof(map<string,vector<SimulatedHit_b1_5*>*>) );
      instance.SetNew(&new_maplEstringcOvectorlESimulatedHit_b1_5mUgRmUgR);
      instance.SetNewArray(&newArray_maplEstringcOvectorlESimulatedHit_b1_5mUgRmUgR);
      instance.SetDelete(&delete_maplEstringcOvectorlESimulatedHit_b1_5mUgRmUgR);
      instance.SetDeleteArray(&deleteArray_maplEstringcOvectorlESimulatedHit_b1_5mUgRmUgR);
      instance.SetDestructor(&destruct_maplEstringcOvectorlESimulatedHit_b1_5mUgRmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<string,vector<SimulatedHit_b1_5*>*> >()));

      ::ROOT::AddClassAlternate("map<string,vector<SimulatedHit_b1_5*>*>","std::map<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::vector<SimulatedHit_b1_5*, std::allocator<SimulatedHit_b1_5*> >*, std::less<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const, std::vector<SimulatedHit_b1_5*, std::allocator<SimulatedHit_b1_5*> >*> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<string,vector<SimulatedHit_b1_5*>*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEstringcOvectorlESimulatedHit_b1_5mUgRmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<string,vector<SimulatedHit_b1_5*>*>*)nullptr)->GetClass();
      maplEstringcOvectorlESimulatedHit_b1_5mUgRmUgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEstringcOvectorlESimulatedHit_b1_5mUgRmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEstringcOvectorlESimulatedHit_b1_5mUgRmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,vector<SimulatedHit_b1_5*>*> : new map<string,vector<SimulatedHit_b1_5*>*>;
   }
   static void *newArray_maplEstringcOvectorlESimulatedHit_b1_5mUgRmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,vector<SimulatedHit_b1_5*>*>[nElements] : new map<string,vector<SimulatedHit_b1_5*>*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEstringcOvectorlESimulatedHit_b1_5mUgRmUgR(void *p) {
      delete ((map<string,vector<SimulatedHit_b1_5*>*>*)p);
   }
   static void deleteArray_maplEstringcOvectorlESimulatedHit_b1_5mUgRmUgR(void *p) {
      delete [] ((map<string,vector<SimulatedHit_b1_5*>*>*)p);
   }
   static void destruct_maplEstringcOvectorlESimulatedHit_b1_5mUgRmUgR(void *p) {
      typedef map<string,vector<SimulatedHit_b1_5*>*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<string,vector<SimulatedHit_b1_5*>*>

namespace ROOT {
   static TClass *maplEstringcOvectorlEReconstructedParticle_b1_5mUgRmUgR_Dictionary();
   static void maplEstringcOvectorlEReconstructedParticle_b1_5mUgRmUgR_TClassManip(TClass*);
   static void *new_maplEstringcOvectorlEReconstructedParticle_b1_5mUgRmUgR(void *p = nullptr);
   static void *newArray_maplEstringcOvectorlEReconstructedParticle_b1_5mUgRmUgR(Long_t size, void *p);
   static void delete_maplEstringcOvectorlEReconstructedParticle_b1_5mUgRmUgR(void *p);
   static void deleteArray_maplEstringcOvectorlEReconstructedParticle_b1_5mUgRmUgR(void *p);
   static void destruct_maplEstringcOvectorlEReconstructedParticle_b1_5mUgRmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<string,vector<ReconstructedParticle_b1_5*>*>*)
   {
      map<string,vector<ReconstructedParticle_b1_5*>*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<string,vector<ReconstructedParticle_b1_5*>*>));
      static ::ROOT::TGenericClassInfo 
         instance("map<string,vector<ReconstructedParticle_b1_5*>*>", -2, "map", 100,
                  typeid(map<string,vector<ReconstructedParticle_b1_5*>*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEstringcOvectorlEReconstructedParticle_b1_5mUgRmUgR_Dictionary, isa_proxy, 0,
                  sizeof(map<string,vector<ReconstructedParticle_b1_5*>*>) );
      instance.SetNew(&new_maplEstringcOvectorlEReconstructedParticle_b1_5mUgRmUgR);
      instance.SetNewArray(&newArray_maplEstringcOvectorlEReconstructedParticle_b1_5mUgRmUgR);
      instance.SetDelete(&delete_maplEstringcOvectorlEReconstructedParticle_b1_5mUgRmUgR);
      instance.SetDeleteArray(&deleteArray_maplEstringcOvectorlEReconstructedParticle_b1_5mUgRmUgR);
      instance.SetDestructor(&destruct_maplEstringcOvectorlEReconstructedParticle_b1_5mUgRmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<string,vector<ReconstructedParticle_b1_5*>*> >()));

      ::ROOT::AddClassAlternate("map<string,vector<ReconstructedParticle_b1_5*>*>","std::map<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::vector<ReconstructedParticle_b1_5*, std::allocator<ReconstructedParticle_b1_5*> >*, std::less<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const, std::vector<ReconstructedParticle_b1_5*, std::allocator<ReconstructedParticle_b1_5*> >*> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<string,vector<ReconstructedParticle_b1_5*>*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEstringcOvectorlEReconstructedParticle_b1_5mUgRmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<string,vector<ReconstructedParticle_b1_5*>*>*)nullptr)->GetClass();
      maplEstringcOvectorlEReconstructedParticle_b1_5mUgRmUgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEstringcOvectorlEReconstructedParticle_b1_5mUgRmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEstringcOvectorlEReconstructedParticle_b1_5mUgRmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,vector<ReconstructedParticle_b1_5*>*> : new map<string,vector<ReconstructedParticle_b1_5*>*>;
   }
   static void *newArray_maplEstringcOvectorlEReconstructedParticle_b1_5mUgRmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,vector<ReconstructedParticle_b1_5*>*>[nElements] : new map<string,vector<ReconstructedParticle_b1_5*>*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEstringcOvectorlEReconstructedParticle_b1_5mUgRmUgR(void *p) {
      delete ((map<string,vector<ReconstructedParticle_b1_5*>*>*)p);
   }
   static void deleteArray_maplEstringcOvectorlEReconstructedParticle_b1_5mUgRmUgR(void *p) {
      delete [] ((map<string,vector<ReconstructedParticle_b1_5*>*>*)p);
   }
   static void destruct_maplEstringcOvectorlEReconstructedParticle_b1_5mUgRmUgR(void *p) {
      typedef map<string,vector<ReconstructedParticle_b1_5*>*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<string,vector<ReconstructedParticle_b1_5*>*>

namespace ROOT {
   static TClass *maplEstringcOvectorlEMcParticle_b1_5mUgRmUgR_Dictionary();
   static void maplEstringcOvectorlEMcParticle_b1_5mUgRmUgR_TClassManip(TClass*);
   static void *new_maplEstringcOvectorlEMcParticle_b1_5mUgRmUgR(void *p = nullptr);
   static void *newArray_maplEstringcOvectorlEMcParticle_b1_5mUgRmUgR(Long_t size, void *p);
   static void delete_maplEstringcOvectorlEMcParticle_b1_5mUgRmUgR(void *p);
   static void deleteArray_maplEstringcOvectorlEMcParticle_b1_5mUgRmUgR(void *p);
   static void destruct_maplEstringcOvectorlEMcParticle_b1_5mUgRmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<string,vector<McParticle_b1_5*>*>*)
   {
      map<string,vector<McParticle_b1_5*>*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<string,vector<McParticle_b1_5*>*>));
      static ::ROOT::TGenericClassInfo 
         instance("map<string,vector<McParticle_b1_5*>*>", -2, "map", 100,
                  typeid(map<string,vector<McParticle_b1_5*>*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEstringcOvectorlEMcParticle_b1_5mUgRmUgR_Dictionary, isa_proxy, 0,
                  sizeof(map<string,vector<McParticle_b1_5*>*>) );
      instance.SetNew(&new_maplEstringcOvectorlEMcParticle_b1_5mUgRmUgR);
      instance.SetNewArray(&newArray_maplEstringcOvectorlEMcParticle_b1_5mUgRmUgR);
      instance.SetDelete(&delete_maplEstringcOvectorlEMcParticle_b1_5mUgRmUgR);
      instance.SetDeleteArray(&deleteArray_maplEstringcOvectorlEMcParticle_b1_5mUgRmUgR);
      instance.SetDestructor(&destruct_maplEstringcOvectorlEMcParticle_b1_5mUgRmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<string,vector<McParticle_b1_5*>*> >()));

      ::ROOT::AddClassAlternate("map<string,vector<McParticle_b1_5*>*>","std::map<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::vector<McParticle_b1_5*, std::allocator<McParticle_b1_5*> >*, std::less<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const, std::vector<McParticle_b1_5*, std::allocator<McParticle_b1_5*> >*> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<string,vector<McParticle_b1_5*>*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEstringcOvectorlEMcParticle_b1_5mUgRmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<string,vector<McParticle_b1_5*>*>*)nullptr)->GetClass();
      maplEstringcOvectorlEMcParticle_b1_5mUgRmUgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEstringcOvectorlEMcParticle_b1_5mUgRmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEstringcOvectorlEMcParticle_b1_5mUgRmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,vector<McParticle_b1_5*>*> : new map<string,vector<McParticle_b1_5*>*>;
   }
   static void *newArray_maplEstringcOvectorlEMcParticle_b1_5mUgRmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,vector<McParticle_b1_5*>*>[nElements] : new map<string,vector<McParticle_b1_5*>*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEstringcOvectorlEMcParticle_b1_5mUgRmUgR(void *p) {
      delete ((map<string,vector<McParticle_b1_5*>*>*)p);
   }
   static void deleteArray_maplEstringcOvectorlEMcParticle_b1_5mUgRmUgR(void *p) {
      delete [] ((map<string,vector<McParticle_b1_5*>*>*)p);
   }
   static void destruct_maplEstringcOvectorlEMcParticle_b1_5mUgRmUgR(void *p) {
      typedef map<string,vector<McParticle_b1_5*>*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<string,vector<McParticle_b1_5*>*>

namespace ROOT {
   static TClass *maplEstringcOvectorlEMcPHelper_b1_5mUgRmUgR_Dictionary();
   static void maplEstringcOvectorlEMcPHelper_b1_5mUgRmUgR_TClassManip(TClass*);
   static void *new_maplEstringcOvectorlEMcPHelper_b1_5mUgRmUgR(void *p = nullptr);
   static void *newArray_maplEstringcOvectorlEMcPHelper_b1_5mUgRmUgR(Long_t size, void *p);
   static void delete_maplEstringcOvectorlEMcPHelper_b1_5mUgRmUgR(void *p);
   static void deleteArray_maplEstringcOvectorlEMcPHelper_b1_5mUgRmUgR(void *p);
   static void destruct_maplEstringcOvectorlEMcPHelper_b1_5mUgRmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<string,vector<McPHelper_b1_5*>*>*)
   {
      map<string,vector<McPHelper_b1_5*>*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<string,vector<McPHelper_b1_5*>*>));
      static ::ROOT::TGenericClassInfo 
         instance("map<string,vector<McPHelper_b1_5*>*>", -2, "map", 100,
                  typeid(map<string,vector<McPHelper_b1_5*>*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEstringcOvectorlEMcPHelper_b1_5mUgRmUgR_Dictionary, isa_proxy, 0,
                  sizeof(map<string,vector<McPHelper_b1_5*>*>) );
      instance.SetNew(&new_maplEstringcOvectorlEMcPHelper_b1_5mUgRmUgR);
      instance.SetNewArray(&newArray_maplEstringcOvectorlEMcPHelper_b1_5mUgRmUgR);
      instance.SetDelete(&delete_maplEstringcOvectorlEMcPHelper_b1_5mUgRmUgR);
      instance.SetDeleteArray(&deleteArray_maplEstringcOvectorlEMcPHelper_b1_5mUgRmUgR);
      instance.SetDestructor(&destruct_maplEstringcOvectorlEMcPHelper_b1_5mUgRmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<string,vector<McPHelper_b1_5*>*> >()));

      ::ROOT::AddClassAlternate("map<string,vector<McPHelper_b1_5*>*>","std::map<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::vector<McPHelper_b1_5*, std::allocator<McPHelper_b1_5*> >*, std::less<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const, std::vector<McPHelper_b1_5*, std::allocator<McPHelper_b1_5*> >*> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<string,vector<McPHelper_b1_5*>*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEstringcOvectorlEMcPHelper_b1_5mUgRmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<string,vector<McPHelper_b1_5*>*>*)nullptr)->GetClass();
      maplEstringcOvectorlEMcPHelper_b1_5mUgRmUgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEstringcOvectorlEMcPHelper_b1_5mUgRmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEstringcOvectorlEMcPHelper_b1_5mUgRmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,vector<McPHelper_b1_5*>*> : new map<string,vector<McPHelper_b1_5*>*>;
   }
   static void *newArray_maplEstringcOvectorlEMcPHelper_b1_5mUgRmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,vector<McPHelper_b1_5*>*>[nElements] : new map<string,vector<McPHelper_b1_5*>*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEstringcOvectorlEMcPHelper_b1_5mUgRmUgR(void *p) {
      delete ((map<string,vector<McPHelper_b1_5*>*>*)p);
   }
   static void deleteArray_maplEstringcOvectorlEMcPHelper_b1_5mUgRmUgR(void *p) {
      delete [] ((map<string,vector<McPHelper_b1_5*>*>*)p);
   }
   static void destruct_maplEstringcOvectorlEMcPHelper_b1_5mUgRmUgR(void *p) {
      typedef map<string,vector<McPHelper_b1_5*>*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<string,vector<McPHelper_b1_5*>*>

namespace ROOT {
   static TClass *maplEstringcOvectorlEDigiForm_b1mUgRmUgR_Dictionary();
   static void maplEstringcOvectorlEDigiForm_b1mUgRmUgR_TClassManip(TClass*);
   static void *new_maplEstringcOvectorlEDigiForm_b1mUgRmUgR(void *p = nullptr);
   static void *newArray_maplEstringcOvectorlEDigiForm_b1mUgRmUgR(Long_t size, void *p);
   static void delete_maplEstringcOvectorlEDigiForm_b1mUgRmUgR(void *p);
   static void deleteArray_maplEstringcOvectorlEDigiForm_b1mUgRmUgR(void *p);
   static void destruct_maplEstringcOvectorlEDigiForm_b1mUgRmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<string,vector<DigiForm_b1*>*>*)
   {
      map<string,vector<DigiForm_b1*>*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<string,vector<DigiForm_b1*>*>));
      static ::ROOT::TGenericClassInfo 
         instance("map<string,vector<DigiForm_b1*>*>", -2, "map", 100,
                  typeid(map<string,vector<DigiForm_b1*>*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEstringcOvectorlEDigiForm_b1mUgRmUgR_Dictionary, isa_proxy, 0,
                  sizeof(map<string,vector<DigiForm_b1*>*>) );
      instance.SetNew(&new_maplEstringcOvectorlEDigiForm_b1mUgRmUgR);
      instance.SetNewArray(&newArray_maplEstringcOvectorlEDigiForm_b1mUgRmUgR);
      instance.SetDelete(&delete_maplEstringcOvectorlEDigiForm_b1mUgRmUgR);
      instance.SetDeleteArray(&deleteArray_maplEstringcOvectorlEDigiForm_b1mUgRmUgR);
      instance.SetDestructor(&destruct_maplEstringcOvectorlEDigiForm_b1mUgRmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<string,vector<DigiForm_b1*>*> >()));

      ::ROOT::AddClassAlternate("map<string,vector<DigiForm_b1*>*>","std::map<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::vector<DigiForm_b1*, std::allocator<DigiForm_b1*> >*, std::less<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const, std::vector<DigiForm_b1*, std::allocator<DigiForm_b1*> >*> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<string,vector<DigiForm_b1*>*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEstringcOvectorlEDigiForm_b1mUgRmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<string,vector<DigiForm_b1*>*>*)nullptr)->GetClass();
      maplEstringcOvectorlEDigiForm_b1mUgRmUgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEstringcOvectorlEDigiForm_b1mUgRmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEstringcOvectorlEDigiForm_b1mUgRmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,vector<DigiForm_b1*>*> : new map<string,vector<DigiForm_b1*>*>;
   }
   static void *newArray_maplEstringcOvectorlEDigiForm_b1mUgRmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,vector<DigiForm_b1*>*>[nElements] : new map<string,vector<DigiForm_b1*>*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEstringcOvectorlEDigiForm_b1mUgRmUgR(void *p) {
      delete ((map<string,vector<DigiForm_b1*>*>*)p);
   }
   static void deleteArray_maplEstringcOvectorlEDigiForm_b1mUgRmUgR(void *p) {
      delete [] ((map<string,vector<DigiForm_b1*>*>*)p);
   }
   static void destruct_maplEstringcOvectorlEDigiForm_b1mUgRmUgR(void *p) {
      typedef map<string,vector<DigiForm_b1*>*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<string,vector<DigiForm_b1*>*>

namespace ROOT {
   static TClass *maplEstringcOvectorlEDStep_b1_5mUgRmUgR_Dictionary();
   static void maplEstringcOvectorlEDStep_b1_5mUgRmUgR_TClassManip(TClass*);
   static void *new_maplEstringcOvectorlEDStep_b1_5mUgRmUgR(void *p = nullptr);
   static void *newArray_maplEstringcOvectorlEDStep_b1_5mUgRmUgR(Long_t size, void *p);
   static void delete_maplEstringcOvectorlEDStep_b1_5mUgRmUgR(void *p);
   static void deleteArray_maplEstringcOvectorlEDStep_b1_5mUgRmUgR(void *p);
   static void destruct_maplEstringcOvectorlEDStep_b1_5mUgRmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<string,vector<DStep_b1_5*>*>*)
   {
      map<string,vector<DStep_b1_5*>*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<string,vector<DStep_b1_5*>*>));
      static ::ROOT::TGenericClassInfo 
         instance("map<string,vector<DStep_b1_5*>*>", -2, "map", 100,
                  typeid(map<string,vector<DStep_b1_5*>*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEstringcOvectorlEDStep_b1_5mUgRmUgR_Dictionary, isa_proxy, 0,
                  sizeof(map<string,vector<DStep_b1_5*>*>) );
      instance.SetNew(&new_maplEstringcOvectorlEDStep_b1_5mUgRmUgR);
      instance.SetNewArray(&newArray_maplEstringcOvectorlEDStep_b1_5mUgRmUgR);
      instance.SetDelete(&delete_maplEstringcOvectorlEDStep_b1_5mUgRmUgR);
      instance.SetDeleteArray(&deleteArray_maplEstringcOvectorlEDStep_b1_5mUgRmUgR);
      instance.SetDestructor(&destruct_maplEstringcOvectorlEDStep_b1_5mUgRmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<string,vector<DStep_b1_5*>*> >()));

      ::ROOT::AddClassAlternate("map<string,vector<DStep_b1_5*>*>","std::map<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::vector<DStep_b1_5*, std::allocator<DStep_b1_5*> >*, std::less<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const, std::vector<DStep_b1_5*, std::allocator<DStep_b1_5*> >*> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<string,vector<DStep_b1_5*>*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEstringcOvectorlEDStep_b1_5mUgRmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<string,vector<DStep_b1_5*>*>*)nullptr)->GetClass();
      maplEstringcOvectorlEDStep_b1_5mUgRmUgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEstringcOvectorlEDStep_b1_5mUgRmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEstringcOvectorlEDStep_b1_5mUgRmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,vector<DStep_b1_5*>*> : new map<string,vector<DStep_b1_5*>*>;
   }
   static void *newArray_maplEstringcOvectorlEDStep_b1_5mUgRmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,vector<DStep_b1_5*>*>[nElements] : new map<string,vector<DStep_b1_5*>*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEstringcOvectorlEDStep_b1_5mUgRmUgR(void *p) {
      delete ((map<string,vector<DStep_b1_5*>*>*)p);
   }
   static void deleteArray_maplEstringcOvectorlEDStep_b1_5mUgRmUgR(void *p) {
      delete [] ((map<string,vector<DStep_b1_5*>*>*)p);
   }
   static void destruct_maplEstringcOvectorlEDStep_b1_5mUgRmUgR(void *p) {
      typedef map<string,vector<DStep_b1_5*>*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<string,vector<DStep_b1_5*>*>

namespace ROOT {
   static TClass *maplEstringcOvectorlECalorimeterHit_b1mUgRmUgR_Dictionary();
   static void maplEstringcOvectorlECalorimeterHit_b1mUgRmUgR_TClassManip(TClass*);
   static void *new_maplEstringcOvectorlECalorimeterHit_b1mUgRmUgR(void *p = nullptr);
   static void *newArray_maplEstringcOvectorlECalorimeterHit_b1mUgRmUgR(Long_t size, void *p);
   static void delete_maplEstringcOvectorlECalorimeterHit_b1mUgRmUgR(void *p);
   static void deleteArray_maplEstringcOvectorlECalorimeterHit_b1mUgRmUgR(void *p);
   static void destruct_maplEstringcOvectorlECalorimeterHit_b1mUgRmUgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<string,vector<CalorimeterHit_b1*>*>*)
   {
      map<string,vector<CalorimeterHit_b1*>*> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<string,vector<CalorimeterHit_b1*>*>));
      static ::ROOT::TGenericClassInfo 
         instance("map<string,vector<CalorimeterHit_b1*>*>", -2, "map", 100,
                  typeid(map<string,vector<CalorimeterHit_b1*>*>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEstringcOvectorlECalorimeterHit_b1mUgRmUgR_Dictionary, isa_proxy, 0,
                  sizeof(map<string,vector<CalorimeterHit_b1*>*>) );
      instance.SetNew(&new_maplEstringcOvectorlECalorimeterHit_b1mUgRmUgR);
      instance.SetNewArray(&newArray_maplEstringcOvectorlECalorimeterHit_b1mUgRmUgR);
      instance.SetDelete(&delete_maplEstringcOvectorlECalorimeterHit_b1mUgRmUgR);
      instance.SetDeleteArray(&deleteArray_maplEstringcOvectorlECalorimeterHit_b1mUgRmUgR);
      instance.SetDestructor(&destruct_maplEstringcOvectorlECalorimeterHit_b1mUgRmUgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<string,vector<CalorimeterHit_b1*>*> >()));

      ::ROOT::AddClassAlternate("map<string,vector<CalorimeterHit_b1*>*>","std::map<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::vector<CalorimeterHit_b1*, std::allocator<CalorimeterHit_b1*> >*, std::less<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const, std::vector<CalorimeterHit_b1*, std::allocator<CalorimeterHit_b1*> >*> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<string,vector<CalorimeterHit_b1*>*>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEstringcOvectorlECalorimeterHit_b1mUgRmUgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<string,vector<CalorimeterHit_b1*>*>*)nullptr)->GetClass();
      maplEstringcOvectorlECalorimeterHit_b1mUgRmUgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEstringcOvectorlECalorimeterHit_b1mUgRmUgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEstringcOvectorlECalorimeterHit_b1mUgRmUgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,vector<CalorimeterHit_b1*>*> : new map<string,vector<CalorimeterHit_b1*>*>;
   }
   static void *newArray_maplEstringcOvectorlECalorimeterHit_b1mUgRmUgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<string,vector<CalorimeterHit_b1*>*>[nElements] : new map<string,vector<CalorimeterHit_b1*>*>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEstringcOvectorlECalorimeterHit_b1mUgRmUgR(void *p) {
      delete ((map<string,vector<CalorimeterHit_b1*>*>*)p);
   }
   static void deleteArray_maplEstringcOvectorlECalorimeterHit_b1mUgRmUgR(void *p) {
      delete [] ((map<string,vector<CalorimeterHit_b1*>*>*)p);
   }
   static void destruct_maplEstringcOvectorlECalorimeterHit_b1mUgRmUgR(void *p) {
      typedef map<string,vector<CalorimeterHit_b1*>*> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<string,vector<CalorimeterHit_b1*>*>

namespace ROOT {
   static TClass *maplEpairlEintcOintgRcOvectorlEDTruthStatemUgRsPgR_Dictionary();
   static void maplEpairlEintcOintgRcOvectorlEDTruthStatemUgRsPgR_TClassManip(TClass*);
   static void *new_maplEpairlEintcOintgRcOvectorlEDTruthStatemUgRsPgR(void *p = nullptr);
   static void *newArray_maplEpairlEintcOintgRcOvectorlEDTruthStatemUgRsPgR(Long_t size, void *p);
   static void delete_maplEpairlEintcOintgRcOvectorlEDTruthStatemUgRsPgR(void *p);
   static void deleteArray_maplEpairlEintcOintgRcOvectorlEDTruthStatemUgRsPgR(void *p);
   static void destruct_maplEpairlEintcOintgRcOvectorlEDTruthStatemUgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<pair<int,int>,vector<DTruthState*> >*)
   {
      map<pair<int,int>,vector<DTruthState*> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<pair<int,int>,vector<DTruthState*> >));
      static ::ROOT::TGenericClassInfo 
         instance("map<pair<int,int>,vector<DTruthState*> >", -2, "map", 100,
                  typeid(map<pair<int,int>,vector<DTruthState*> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEpairlEintcOintgRcOvectorlEDTruthStatemUgRsPgR_Dictionary, isa_proxy, 4,
                  sizeof(map<pair<int,int>,vector<DTruthState*> >) );
      instance.SetNew(&new_maplEpairlEintcOintgRcOvectorlEDTruthStatemUgRsPgR);
      instance.SetNewArray(&newArray_maplEpairlEintcOintgRcOvectorlEDTruthStatemUgRsPgR);
      instance.SetDelete(&delete_maplEpairlEintcOintgRcOvectorlEDTruthStatemUgRsPgR);
      instance.SetDeleteArray(&deleteArray_maplEpairlEintcOintgRcOvectorlEDTruthStatemUgRsPgR);
      instance.SetDestructor(&destruct_maplEpairlEintcOintgRcOvectorlEDTruthStatemUgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<pair<int,int>,vector<DTruthState*> > >()));

      ::ROOT::AddClassAlternate("map<pair<int,int>,vector<DTruthState*> >","std::map<std::pair<int, int>, std::vector<DTruthState*, std::allocator<DTruthState*> >, std::less<std::pair<int, int> >, std::allocator<std::pair<std::pair<int, int> const, std::vector<DTruthState*, std::allocator<DTruthState*> > > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<pair<int,int>,vector<DTruthState*> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEpairlEintcOintgRcOvectorlEDTruthStatemUgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<pair<int,int>,vector<DTruthState*> >*)nullptr)->GetClass();
      maplEpairlEintcOintgRcOvectorlEDTruthStatemUgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEpairlEintcOintgRcOvectorlEDTruthStatemUgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEpairlEintcOintgRcOvectorlEDTruthStatemUgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<pair<int,int>,vector<DTruthState*> > : new map<pair<int,int>,vector<DTruthState*> >;
   }
   static void *newArray_maplEpairlEintcOintgRcOvectorlEDTruthStatemUgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<pair<int,int>,vector<DTruthState*> >[nElements] : new map<pair<int,int>,vector<DTruthState*> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEpairlEintcOintgRcOvectorlEDTruthStatemUgRsPgR(void *p) {
      delete ((map<pair<int,int>,vector<DTruthState*> >*)p);
   }
   static void deleteArray_maplEpairlEintcOintgRcOvectorlEDTruthStatemUgRsPgR(void *p) {
      delete [] ((map<pair<int,int>,vector<DTruthState*> >*)p);
   }
   static void destruct_maplEpairlEintcOintgRcOvectorlEDTruthStatemUgRsPgR(void *p) {
      typedef map<pair<int,int>,vector<DTruthState*> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<pair<int,int>,vector<DTruthState*> >

namespace ROOT {
   static TClass *maplEpairlEintcOintgRcOintgR_Dictionary();
   static void maplEpairlEintcOintgRcOintgR_TClassManip(TClass*);
   static void *new_maplEpairlEintcOintgRcOintgR(void *p = nullptr);
   static void *newArray_maplEpairlEintcOintgRcOintgR(Long_t size, void *p);
   static void delete_maplEpairlEintcOintgRcOintgR(void *p);
   static void deleteArray_maplEpairlEintcOintgRcOintgR(void *p);
   static void destruct_maplEpairlEintcOintgRcOintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<pair<int,int>,int>*)
   {
      map<pair<int,int>,int> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<pair<int,int>,int>));
      static ::ROOT::TGenericClassInfo 
         instance("map<pair<int,int>,int>", -2, "map", 100,
                  typeid(map<pair<int,int>,int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEpairlEintcOintgRcOintgR_Dictionary, isa_proxy, 0,
                  sizeof(map<pair<int,int>,int>) );
      instance.SetNew(&new_maplEpairlEintcOintgRcOintgR);
      instance.SetNewArray(&newArray_maplEpairlEintcOintgRcOintgR);
      instance.SetDelete(&delete_maplEpairlEintcOintgRcOintgR);
      instance.SetDeleteArray(&deleteArray_maplEpairlEintcOintgRcOintgR);
      instance.SetDestructor(&destruct_maplEpairlEintcOintgRcOintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<pair<int,int>,int> >()));

      ::ROOT::AddClassAlternate("map<pair<int,int>,int>","std::map<std::pair<int, int>, int, std::less<std::pair<int, int> >, std::allocator<std::pair<std::pair<int, int> const, int> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<pair<int,int>,int>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEpairlEintcOintgRcOintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<pair<int,int>,int>*)nullptr)->GetClass();
      maplEpairlEintcOintgRcOintgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEpairlEintcOintgRcOintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEpairlEintcOintgRcOintgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<pair<int,int>,int> : new map<pair<int,int>,int>;
   }
   static void *newArray_maplEpairlEintcOintgRcOintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<pair<int,int>,int>[nElements] : new map<pair<int,int>,int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEpairlEintcOintgRcOintgR(void *p) {
      delete ((map<pair<int,int>,int>*)p);
   }
   static void deleteArray_maplEpairlEintcOintgRcOintgR(void *p) {
      delete [] ((map<pair<int,int>,int>*)p);
   }
   static void destruct_maplEpairlEintcOintgRcOintgR(void *p) {
      typedef map<pair<int,int>,int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<pair<int,int>,int>

namespace ROOT {
   static TClass *maplEpairlEintcOintgRcOfloatgR_Dictionary();
   static void maplEpairlEintcOintgRcOfloatgR_TClassManip(TClass*);
   static void *new_maplEpairlEintcOintgRcOfloatgR(void *p = nullptr);
   static void *newArray_maplEpairlEintcOintgRcOfloatgR(Long_t size, void *p);
   static void delete_maplEpairlEintcOintgRcOfloatgR(void *p);
   static void deleteArray_maplEpairlEintcOintgRcOfloatgR(void *p);
   static void destruct_maplEpairlEintcOintgRcOfloatgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<pair<int,int>,float>*)
   {
      map<pair<int,int>,float> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<pair<int,int>,float>));
      static ::ROOT::TGenericClassInfo 
         instance("map<pair<int,int>,float>", -2, "map", 100,
                  typeid(map<pair<int,int>,float>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEpairlEintcOintgRcOfloatgR_Dictionary, isa_proxy, 0,
                  sizeof(map<pair<int,int>,float>) );
      instance.SetNew(&new_maplEpairlEintcOintgRcOfloatgR);
      instance.SetNewArray(&newArray_maplEpairlEintcOintgRcOfloatgR);
      instance.SetDelete(&delete_maplEpairlEintcOintgRcOfloatgR);
      instance.SetDeleteArray(&deleteArray_maplEpairlEintcOintgRcOfloatgR);
      instance.SetDestructor(&destruct_maplEpairlEintcOintgRcOfloatgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<pair<int,int>,float> >()));

      ::ROOT::AddClassAlternate("map<pair<int,int>,float>","std::map<std::pair<int, int>, float, std::less<std::pair<int, int> >, std::allocator<std::pair<std::pair<int, int> const, float> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<pair<int,int>,float>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEpairlEintcOintgRcOfloatgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<pair<int,int>,float>*)nullptr)->GetClass();
      maplEpairlEintcOintgRcOfloatgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEpairlEintcOintgRcOfloatgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEpairlEintcOintgRcOfloatgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<pair<int,int>,float> : new map<pair<int,int>,float>;
   }
   static void *newArray_maplEpairlEintcOintgRcOfloatgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<pair<int,int>,float>[nElements] : new map<pair<int,int>,float>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEpairlEintcOintgRcOfloatgR(void *p) {
      delete ((map<pair<int,int>,float>*)p);
   }
   static void deleteArray_maplEpairlEintcOintgRcOfloatgR(void *p) {
      delete [] ((map<pair<int,int>,float>*)p);
   }
   static void destruct_maplEpairlEintcOintgRcOfloatgR(void *p) {
      typedef map<pair<int,int>,float> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<pair<int,int>,float>

namespace ROOT {
   static TClass *maplEintcOintgR_Dictionary();
   static void maplEintcOintgR_TClassManip(TClass*);
   static void *new_maplEintcOintgR(void *p = nullptr);
   static void *newArray_maplEintcOintgR(Long_t size, void *p);
   static void delete_maplEintcOintgR(void *p);
   static void deleteArray_maplEintcOintgR(void *p);
   static void destruct_maplEintcOintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<int,int>*)
   {
      map<int,int> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<int,int>));
      static ::ROOT::TGenericClassInfo 
         instance("map<int,int>", -2, "map", 100,
                  typeid(map<int,int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEintcOintgR_Dictionary, isa_proxy, 0,
                  sizeof(map<int,int>) );
      instance.SetNew(&new_maplEintcOintgR);
      instance.SetNewArray(&newArray_maplEintcOintgR);
      instance.SetDelete(&delete_maplEintcOintgR);
      instance.SetDeleteArray(&deleteArray_maplEintcOintgR);
      instance.SetDestructor(&destruct_maplEintcOintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<int,int> >()));

      ::ROOT::AddClassAlternate("map<int,int>","std::map<int, int, std::less<int>, std::allocator<std::pair<int const, int> > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<int,int>*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEintcOintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<int,int>*)nullptr)->GetClass();
      maplEintcOintgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEintcOintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEintcOintgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,int> : new map<int,int>;
   }
   static void *newArray_maplEintcOintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<int,int>[nElements] : new map<int,int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEintcOintgR(void *p) {
      delete ((map<int,int>*)p);
   }
   static void deleteArray_maplEintcOintgR(void *p) {
      delete [] ((map<int,int>*)p);
   }
   static void destruct_maplEintcOintgR(void *p) {
      typedef map<int,int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<int,int>

namespace ROOT {
   static TClass *maplEDTruthcLcLDTruthDetPVcOpairlEfloatcOfloatgRsPgR_Dictionary();
   static void maplEDTruthcLcLDTruthDetPVcOpairlEfloatcOfloatgRsPgR_TClassManip(TClass*);
   static void *new_maplEDTruthcLcLDTruthDetPVcOpairlEfloatcOfloatgRsPgR(void *p = nullptr);
   static void *newArray_maplEDTruthcLcLDTruthDetPVcOpairlEfloatcOfloatgRsPgR(Long_t size, void *p);
   static void delete_maplEDTruthcLcLDTruthDetPVcOpairlEfloatcOfloatgRsPgR(void *p);
   static void deleteArray_maplEDTruthcLcLDTruthDetPVcOpairlEfloatcOfloatgRsPgR(void *p);
   static void destruct_maplEDTruthcLcLDTruthDetPVcOpairlEfloatcOfloatgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<DTruth::DTruthDetPV,pair<float,float> >*)
   {
      map<DTruth::DTruthDetPV,pair<float,float> > *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<DTruth::DTruthDetPV,pair<float,float> >));
      static ::ROOT::TGenericClassInfo 
         instance("map<DTruth::DTruthDetPV,pair<float,float> >", -2, "map", 100,
                  typeid(map<DTruth::DTruthDetPV,pair<float,float> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEDTruthcLcLDTruthDetPVcOpairlEfloatcOfloatgRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(map<DTruth::DTruthDetPV,pair<float,float> >) );
      instance.SetNew(&new_maplEDTruthcLcLDTruthDetPVcOpairlEfloatcOfloatgRsPgR);
      instance.SetNewArray(&newArray_maplEDTruthcLcLDTruthDetPVcOpairlEfloatcOfloatgRsPgR);
      instance.SetDelete(&delete_maplEDTruthcLcLDTruthDetPVcOpairlEfloatcOfloatgRsPgR);
      instance.SetDeleteArray(&deleteArray_maplEDTruthcLcLDTruthDetPVcOpairlEfloatcOfloatgRsPgR);
      instance.SetDestructor(&destruct_maplEDTruthcLcLDTruthDetPVcOpairlEfloatcOfloatgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<DTruth::DTruthDetPV,pair<float,float> > >()));

      ::ROOT::AddClassAlternate("map<DTruth::DTruthDetPV,pair<float,float> >","std::map<DTruth::DTruthDetPV, std::pair<float, float>, std::less<DTruth::DTruthDetPV>, std::allocator<std::pair<DTruth::DTruthDetPV const, std::pair<float, float> > > >");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<DTruth::DTruthDetPV,pair<float,float> >*)nullptr); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEDTruthcLcLDTruthDetPVcOpairlEfloatcOfloatgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<DTruth::DTruthDetPV,pair<float,float> >*)nullptr)->GetClass();
      maplEDTruthcLcLDTruthDetPVcOpairlEfloatcOfloatgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEDTruthcLcLDTruthDetPVcOpairlEfloatcOfloatgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEDTruthcLcLDTruthDetPVcOpairlEfloatcOfloatgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<DTruth::DTruthDetPV,pair<float,float> > : new map<DTruth::DTruthDetPV,pair<float,float> >;
   }
   static void *newArray_maplEDTruthcLcLDTruthDetPVcOpairlEfloatcOfloatgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<DTruth::DTruthDetPV,pair<float,float> >[nElements] : new map<DTruth::DTruthDetPV,pair<float,float> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEDTruthcLcLDTruthDetPVcOpairlEfloatcOfloatgRsPgR(void *p) {
      delete ((map<DTruth::DTruthDetPV,pair<float,float> >*)p);
   }
   static void deleteArray_maplEDTruthcLcLDTruthDetPVcOpairlEfloatcOfloatgRsPgR(void *p) {
      delete [] ((map<DTruth::DTruthDetPV,pair<float,float> >*)p);
   }
   static void destruct_maplEDTruthcLcLDTruthDetPVcOpairlEfloatcOfloatgRsPgR(void *p) {
      typedef map<DTruth::DTruthDetPV,pair<float,float> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<DTruth::DTruthDetPV,pair<float,float> >

namespace {
  void TriggerDictionaryInitialization_libDEvent_Impl() {
    static const char* headers[] = {
"/tmp/source/Utility/Object/include/Object/DHit.h",
"/tmp/source/Utility/Object/include/Object/DParticle.h",
"/tmp/source/Utility/Object/include/Object/DStep.h",
"/tmp/source/Utility/Object/include/Object/SimulatedHit.h",
"/tmp/source/Utility/Object/include/Object/CalorimeterHit.h",
"/tmp/source/Utility/Object/include/Object/McParticle.h",
"/tmp/source/Utility/Object/include/Object/ReconstructedParticle.h",
"/tmp/source/Utility/Object/include/Object/DigiForm.hh",
"/tmp/source/Utility/Object/include/Object/DEvent.h",
"/tmp/source/Utility/Object/include/Object/DMagnet.h",
"/tmp/source/Utility/Object/include/Object/DDetectorIDMaps.h",
"/tmp/source/Utility/Object/include/Object/McPHelper.h",
"/tmp/source/Utility/Object/include/Object/DTruth.h",
"/tmp/source/Utility/Object/include/Object/versions/DHit_b1_4.h",
"/tmp/source/Utility/Object/include/Object/versions/DParticle_b1_5.h",
"/tmp/source/Utility/Object/include/Object/versions/DStep_b1_5.h",
"/tmp/source/Utility/Object/include/Object/versions/SimulatedHit_b1_5.h",
"/tmp/source/Utility/Object/include/Object/versions/CalorimeterHit_b1.h",
"/tmp/source/Utility/Object/include/Object/versions/McParticle_b1_5.h",
"/tmp/source/Utility/Object/include/Object/versions/ReconstructedParticle_b1_5.h",
"/tmp/source/Utility/Object/include/Object/versions/DigiForm_b1.hh",
"/tmp/source/Utility/Object/include/Object/versions/DEvent_b1_5.h",
"/tmp/source/Utility/Object/include/Object/versions/DMagnet_b1.h",
"/tmp/source/Utility/Object/include/Object/versions/DDetectorIDMaps_b1_5.h",
"/tmp/source/Utility/Object/include/Object/versions/McPHelper_b1_5.h",
nullptr
    };
    static const char* includePaths[] = {
"/opt/geant4/include/Geant4",
"/usr/include",
"/usr/include/eigen3",
"/opt/root/include",
"/tmp/source/.",
"/tmp/source/GenFit2/core/include",
"/tmp/source/GenFit2/fields/include",
"/tmp/source/GenFit2/fitters/include",
"/tmp/source/GenFit2/measurements/include",
"/tmp/source/GenFit2/trackReps/include",
"/tmp/source/Utility/Object/include",
"/tmp/source/Utility/Object/include/Object",
"/tmp/source/Utility/UTIL/include",
"/tmp/source/Algorithms/CutFlowAnalysis/include",
"/tmp/source/Algorithms/Digitizer/include",
"/tmp/source/Algorithms/ExampleProcessor/include",
"/tmp/source/Algorithms/MCTruthAnalysis/include",
"/tmp/source/Algorithms/RecECAL/include",
"/tmp/source/Algorithms/Tracking/include",
"/tmp/source/Algorithms/Tracking/include/Algo",
"/tmp/source/Utility/Object",
"/opt/root/include/",
"/tmp/source/cmake-build-debug-docker/Utility/Object/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libDEvent dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$Object/versions/DStep_b1_5.h")))  __attribute__((annotate("$clingAutoload$/tmp/source/Utility/Object/include/Object/DStep.h")))  DStep_b1_5;
namespace std{template <typename _Tp> class __attribute__((annotate("$clingAutoload$bits/allocator.h")))  __attribute__((annotate("$clingAutoload$string")))  allocator;
}
class __attribute__((annotate("$clingAutoload$Object/versions/McParticle_b1_5.h")))  __attribute__((annotate("$clingAutoload$/tmp/source/Utility/Object/include/Object/SimulatedHit.h")))  McParticle_b1_5;
class __attribute__((annotate("$clingAutoload$Object/versions/McPHelper_b1_5.h")))  __attribute__((annotate("$clingAutoload$/tmp/source/Utility/Object/include/Object/DEvent.h")))  McPHelper_b1_5;
class __attribute__((annotate("$clingAutoload$Object/versions/ReconstructedParticle_b1_5.h")))  __attribute__((annotate("$clingAutoload$/tmp/source/Utility/Object/include/Object/SimulatedHit.h")))  ReconstructedParticle_b1_5;
class __attribute__((annotate("$clingAutoload$Object/versions/SimulatedHit_b1_5.h")))  __attribute__((annotate("$clingAutoload$/tmp/source/Utility/Object/include/Object/SimulatedHit.h")))  SimulatedHit_b1_5;
class __attribute__((annotate("$clingAutoload$Object/versions/CalorimeterHit_b1.h")))  __attribute__((annotate("$clingAutoload$/tmp/source/Utility/Object/include/Object/SimulatedHit.h")))  CalorimeterHit_b1;
class __attribute__((annotate("$clingAutoload$Object/versions/DigiForm_b1.hh")))  __attribute__((annotate("$clingAutoload$/tmp/source/Utility/Object/include/Object/DigiForm.hh")))  DigiForm_b1;
class __attribute__((annotate("$clingAutoload$Object/DTruth.h")))  __attribute__((annotate("$clingAutoload$/tmp/source/Utility/Object/include/Object/DEvent.h")))  DTruthParticle;
class __attribute__((annotate("$clingAutoload$TVector3.h")))  __attribute__((annotate("$clingAutoload$/tmp/source/Utility/Object/include/Object/DDetectorIDMaps.h")))  TVector3;
class __attribute__((annotate("$clingAutoload$Object/versions/DParticle_b1_5.h")))  __attribute__((annotate("$clingAutoload$/tmp/source/Utility/Object/include/Object/DParticle.h")))  DParticle_b1_5;
class __attribute__((annotate("$clingAutoload$Object/versions/DHit_b1_4.h")))  __attribute__((annotate("$clingAutoload$/tmp/source/Utility/Object/include/Object/DHit.h")))  DHit_b1_4;
namespace std{template <typename _T1, typename _T2> struct __attribute__((annotate("$clingAutoload$bits/stl_iterator.h")))  __attribute__((annotate("$clingAutoload$string")))  pair;
}
class __attribute__((annotate("$clingAutoload$Object/DTruth.h")))  __attribute__((annotate("$clingAutoload$/tmp/source/Utility/Object/include/Object/DEvent.h")))  DTruthState;
struct __attribute__((annotate("$clingAutoload$Utility/PhysicsProcessDef.h")))  __attribute__((annotate("$clingAutoload$/tmp/source/Utility/Object/include/Object/DEvent.h")))  PhysicsDef;
class __attribute__((annotate("$clingAutoload$Object/DTruth.h")))  __attribute__((annotate("$clingAutoload$/tmp/source/Utility/Object/include/Object/DEvent.h")))  DTruthProcess;
class __attribute__((annotate("$clingAutoload$Object/DTruth.h")))  __attribute__((annotate("$clingAutoload$/tmp/source/Utility/Object/include/Object/DEvent.h")))  DTruth;
class __attribute__((annotate("$clingAutoload$Object/versions/DEvent_b1_5.h")))  __attribute__((annotate("$clingAutoload$/tmp/source/Utility/Object/include/Object/DEvent.h")))  DEvent_b1_5;
class __attribute__((annotate("$clingAutoload$Object/versions/DMagnet_b1.h")))  __attribute__((annotate("$clingAutoload$/tmp/source/Utility/Object/include/Object/DMagnet.h")))  DMagnet_b1;
class __attribute__((annotate("$clingAutoload$Object/versions/DDetectorIDMaps_b1_5.h")))  __attribute__((annotate("$clingAutoload$/tmp/source/Utility/Object/include/Object/DDetectorIDMaps.h")))  DDetectorIDMaps_b1_5;
using DHit __attribute__((annotate("$clingAutoload$/tmp/source/Utility/Object/include/Object/DHit.h")))  = DHit_b1_4;
using DParticle __attribute__((annotate("$clingAutoload$/tmp/source/Utility/Object/include/Object/DParticle.h")))  = DParticle_b1_5;
using DStep __attribute__((annotate("$clingAutoload$/tmp/source/Utility/Object/include/Object/DStep.h")))  = DStep_b1_5;
using CalorimeterHit __attribute__((annotate("$clingAutoload$Object/CalorimeterHit.h")))  __attribute__((annotate("$clingAutoload$/tmp/source/Utility/Object/include/Object/SimulatedHit.h")))  = CalorimeterHit_b1;
using ReconstructedParticle __attribute__((annotate("$clingAutoload$Object/ReconstructedParticle.h")))  __attribute__((annotate("$clingAutoload$/tmp/source/Utility/Object/include/Object/SimulatedHit.h")))  = ReconstructedParticle_b1_5;
using McParticle __attribute__((annotate("$clingAutoload$Object/McParticle.h")))  __attribute__((annotate("$clingAutoload$/tmp/source/Utility/Object/include/Object/SimulatedHit.h")))  = McParticle_b1_5;
using SimulatedHit __attribute__((annotate("$clingAutoload$/tmp/source/Utility/Object/include/Object/SimulatedHit.h")))  = SimulatedHit_b1_5;
using DigiForm __attribute__((annotate("$clingAutoload$/tmp/source/Utility/Object/include/Object/DigiForm.hh")))  = DigiForm_b1;
using McPHelper __attribute__((annotate("$clingAutoload$Object/McPHelper.h")))  __attribute__((annotate("$clingAutoload$/tmp/source/Utility/Object/include/Object/DEvent.h")))  = McPHelper_b1_5;
using DEvent __attribute__((annotate("$clingAutoload$/tmp/source/Utility/Object/include/Object/DEvent.h")))  = DEvent_b1_5;
using DMagnet __attribute__((annotate("$clingAutoload$/tmp/source/Utility/Object/include/Object/DMagnet.h")))  = DMagnet_b1;
using DDetectorIDMaps __attribute__((annotate("$clingAutoload$/tmp/source/Utility/Object/include/Object/DDetectorIDMaps.h")))  = DDetectorIDMaps_b1_5;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libDEvent dictionary payload"

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
#include "/tmp/source/Utility/Object/include/Object/DHit.h"
#include "/tmp/source/Utility/Object/include/Object/DParticle.h"
#include "/tmp/source/Utility/Object/include/Object/DStep.h"
#include "/tmp/source/Utility/Object/include/Object/SimulatedHit.h"
#include "/tmp/source/Utility/Object/include/Object/CalorimeterHit.h"
#include "/tmp/source/Utility/Object/include/Object/McParticle.h"
#include "/tmp/source/Utility/Object/include/Object/ReconstructedParticle.h"
#include "/tmp/source/Utility/Object/include/Object/DigiForm.hh"
#include "/tmp/source/Utility/Object/include/Object/DEvent.h"
#include "/tmp/source/Utility/Object/include/Object/DMagnet.h"
#include "/tmp/source/Utility/Object/include/Object/DDetectorIDMaps.h"
#include "/tmp/source/Utility/Object/include/Object/McPHelper.h"
#include "/tmp/source/Utility/Object/include/Object/DTruth.h"
#include "/tmp/source/Utility/Object/include/Object/versions/DHit_b1_4.h"
#include "/tmp/source/Utility/Object/include/Object/versions/DParticle_b1_5.h"
#include "/tmp/source/Utility/Object/include/Object/versions/DStep_b1_5.h"
#include "/tmp/source/Utility/Object/include/Object/versions/SimulatedHit_b1_5.h"
#include "/tmp/source/Utility/Object/include/Object/versions/CalorimeterHit_b1.h"
#include "/tmp/source/Utility/Object/include/Object/versions/McParticle_b1_5.h"
#include "/tmp/source/Utility/Object/include/Object/versions/ReconstructedParticle_b1_5.h"
#include "/tmp/source/Utility/Object/include/Object/versions/DigiForm_b1.hh"
#include "/tmp/source/Utility/Object/include/Object/versions/DEvent_b1_5.h"
#include "/tmp/source/Utility/Object/include/Object/versions/DMagnet_b1.h"
#include "/tmp/source/Utility/Object/include/Object/versions/DDetectorIDMaps_b1_5.h"
#include "/tmp/source/Utility/Object/include/Object/versions/McPHelper_b1_5.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"CalorimeterHit", payloadCode, "@",
"CalorimeterHit_b1", payloadCode, "@",
"DDetectorIDMaps", payloadCode, "@",
"DDetectorIDMaps_b1_5", payloadCode, "@",
"DEvent", payloadCode, "@",
"DEvent_b1_5", payloadCode, "@",
"DHit", payloadCode, "@",
"DHit_b1_4", payloadCode, "@",
"DMagnet", payloadCode, "@",
"DMagnet::MagSlice", payloadCode, "@",
"DMagnet_b1", payloadCode, "@",
"DParticle", payloadCode, "@",
"DParticle_b1_5", payloadCode, "@",
"DStep", payloadCode, "@",
"DStep_b1_5", payloadCode, "@",
"DTruth", payloadCode, "@",
"DTruthParticle", payloadCode, "@",
"DTruthProcess", payloadCode, "@",
"DTruthState", payloadCode, "@",
"DigiForm", payloadCode, "@",
"DigiForm_b1", payloadCode, "@",
"McPHelper", payloadCode, "@",
"McPHelper_b1_5", payloadCode, "@",
"McParticle", payloadCode, "@",
"McParticle_b1_5", payloadCode, "@",
"PhysicsDef", payloadCode, "@",
"ReconstructedParticle", payloadCode, "@",
"ReconstructedParticle_b1_5", payloadCode, "@",
"SimulatedHit", payloadCode, "@",
"SimulatedHit_b1_5", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libDEvent",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libDEvent_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libDEvent_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libDEvent() {
  TriggerDictionaryInitialization_libDEvent_Impl();
}
