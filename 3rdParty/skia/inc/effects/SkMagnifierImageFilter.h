/*
 * Copyright 2012 The Android Open Source Project
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef SkMagnifierImageFilter_DEFINED
#define SkMagnifierImageFilter_DEFINED

#include "SkRect.h"
#include "SkImageFilter.h"

class SK_API SkMagnifierImageFilter : public SkImageFilter {
public:
    static SkImageFilter* Create(const SkRect& src, SkScalar inset, SkImageFilter* input = NULL);

    SK_DECLARE_PUBLIC_FLATTENABLE_DESERIALIZATION_PROCS(SkMagnifierImageFilter)

protected:
    SkMagnifierImageFilter(const SkRect& srcRect, SkScalar inset, SkImageFilter* input);
#ifdef SK_SUPPORT_LEGACY_DEEPFLATTENING
    explicit SkMagnifierImageFilter(SkReadBuffer& buffer);
#endif
    virtual void flatten(SkWriteBuffer&) const SK_OVERRIDE;

    virtual bool onFilterImage(Proxy*, const SkBitmap& src, const Context&,
                               SkBitmap* result, SkIPoint* offset) const SK_OVERRIDE;
#if SK_SUPPORT_GPU
    virtual bool asNewEffect(GrEffect** effect, GrTexture* texture, const SkMatrix& matrix,
                             const SkIRect& bounds) const SK_OVERRIDE;
#endif

private:
    SkRect fSrcRect;
    SkScalar fInset;
    typedef SkImageFilter INHERITED;
};

#endif
