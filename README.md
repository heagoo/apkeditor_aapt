# AAPT for APK Editor
Successfully Compiled with r12b.
APK Editor: https://play.google.com/store/apps/details?id=com.gmail.heagoo.apkeditor

Modification includes:

diff ./base/tools/aapt/Resource.cpp aapt/jni/Resource.cpp
318a319
>                     || (*str >= 'A' && *str <= 'Z')
319a321
>                     || *str == '$'
321c323
<                 fprintf(stderr, "%s: Invalid file name: must contain only [a-z0-9_.]\n",
---
>                 fprintf(stderr, "%s: Invalid file name: must contain only [a-zA-Z0-9_.]\n",
1764c1766
<     sp<AaptFile> outManifestFile = new AaptFile(manifestFile->getSourceFile(),
---
>     /*sp<AaptFile> outManifestFile = new AaptFile(manifestFile->getSourceFile(),
2028c2030
<     }
---
>     }*/


diff ./base/tools/aapt/StringPool.cpp aapt/jni/StringPool.cpp
351a352
>     int num_remove = 0;
359c360,364
<         newEntryStyleArray.removeAt(i);
---
>         //newEntryStyleArray.removeAt(i);
>         num_remove += 1;
>     }
>     if (num_remove > 0) {
>         newEntryStyleArray.removeItemsAt(newEntryStyleArray.size() - num_remove, num_remove);


            diff ./base/tools/aapt/ResourceTable.cpp aapt/jni/ResourceTable.cpp
2650,2651c2650,2651
<             LOG_ALWAYS_FATAL_IF(ti == 0 && attr != t,
<                                 "First type is not attr!");
---
>             //LOG_ALWAYS_FATAL_IF(ti == 0 && attr != t,
>             //                    "First type is not attr!");
4037a4038
>             if (e == nullptr) { continue; }
4048,4049c4049,4051
<                     origOrder.removeAt(i);
<                     N--;
---
>                     //origOrder.removeAt(i);
>                     //N--;
>                     origOrder.editItemAt(i) = nullptr;
