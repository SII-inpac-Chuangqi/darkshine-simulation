//
// Created by Yulei on 2023/3/14.
// Copyright (c) 2023 Yulei. All rights reserved.
//

#include "Animation/AnimationData.h"

// Required by Singleton
AnimationData *pAniData = nullptr;

// Get Instance Class
AnimationData *AnimationData::CreateInstance() {
    if (pAniData == nullptr)
        pAniData = new AnimationData();

    return pAniData;
}

AnimationData::AnimationData() {
}

