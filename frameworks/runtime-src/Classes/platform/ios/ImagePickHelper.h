
//  Created by apple_lee on 2016/10/26.
//  Copyright © 2016年 apple_lee. All rights reserved.

#import <Foundation/Foundation.h>
#ifndef CC_TARGET_OS_MAC
typedef void(^SelectBlock)(NSString *result);

@interface ImagePickHelper : NSObject<UINavigationControllerDelegate,UIImagePickerControllerDelegate>
{
    int _callbackId;
 
 
}
+ (instancetype)sharedInstance;

-(void) start:(NSDictionary*) dict;


//打开相册
- (void) openCamera;
//打开图库
- (void) openPhotoAlbum;
//打开相册
- (void) openPhotoLibrary;

- (void) openImagePickerByType:(UIImagePickerControllerSourceType)sourceType allowEditing:(BOOL)allowEditing;


-(void) openSelector:(BOOL)allowEditing;

- (void) setSelectBlock:(SelectBlock)block;
- (void) callUploadToLua:(NSString*) path;

@end
#endif
