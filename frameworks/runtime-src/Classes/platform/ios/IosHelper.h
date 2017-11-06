//
//  IosHelper.h
//  wlhall_libs
//
//  Created by apple_lee on 2016/10/26.
//  Copyright © 2016年 apple_lee. All rights reserved.
//

@interface IosHelper : NSObject

+(NSArray*) urlSchemesByName:(NSString*) urlname;
#ifndef CC_TARGET_OS_MAC
+ (UIImage *) scaleFromImage: (UIImage *) image toSize: (CGSize) size;

+ (UIImage *)thumbnailWithImageWithoutScale:(UIImage *)image size:(CGSize)asize;
#endif
@end

int IosCallLuaFunction(int handler ,int nValue1,const char* strValue2,const char* strValue3,bool release);
