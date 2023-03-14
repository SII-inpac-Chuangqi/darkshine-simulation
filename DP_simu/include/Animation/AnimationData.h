//
// Created by Yulei on 2023/3/14.
// Copyright (c) 2023 Yulei. All rights reserved.
//

#ifndef DSIMU_ANIMATIONDATA_H
#define DSIMU_ANIMATIONDATA_H

class AnimationData {

public:
    AnimationData(const AnimationData &) = delete;

    AnimationData &operator=(AnimationData const &) = delete;

    static AnimationData *CreateInstance();

private:
    AnimationData();
};

extern AnimationData *pAniData;
#endif //DSIMU_ANIMATIONDATA_H
