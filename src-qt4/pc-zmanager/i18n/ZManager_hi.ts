<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.0">
<context>
    <name>DialogFSProp</name>
    <message>
        <source>Dialog</source>
        <translation>संवाद
            </translation>
    </message>
    <message>
        <source>Apply changes recursively to all descendants</source>
        <translation>सभी वंश में बारी बारी से परिवर्तन लागू करें
            </translation>
    </message>
    <message>
        <source>Property</source>
        <translation>संपत्ति
            </translation>
    </message>
    <message>
        <source>Value</source>
        <translation>मूल्य
            </translation>
    </message>
    <message>
        <source>Source</source>
        <translation>स्रोत
            </translation>
    </message>
    <message>
        <source>Add user property</source>
        <translation>उपयोगकर्ता संपत्ति जोड़ें
            </translation>
    </message>
    <message>
        <source>...</source>
        <translation>...
            </translation>
    </message>
    <message>
        <source>Remove user property</source>
        <translation>उपयोगकर्ता संपत्ति निकालें
            </translation>
    </message>
    <message>
        <source>Inherit from parent</source>
        <translation>उत्पादक से विरासत में लें
            </translation>
    </message>
    <message>
        <source>Revert changes</source>
        <translation>परिवर्तन आना
            </translation>
    </message>
    <message>
        <source>Controls how ACL entries are inherited when files and directories are created. The values are
discard, noallow, restricted, and passthrough. For a description of these values, see ACL Properties.</source>
        <translation>नियंत्रित करता है के कैसे एसीएल प्रविष्टियों विरासत में मिल रही हैं, जब फ़ाइलें और निर्देशिका का निर्माण होता है | मूल्य हैं 
 त्यागना, अस्वीकारना, प्रतिबंधित, और पासथ्रू | इन मूल्यों का वर्णन के जान ने के लिए, एसीएल प्रॉपर्टीस देखें |
            </translation>
    </message>
    <message>
        <source>Controls how an ACL entry is modified during a chmod operation.
The values are discard, groupmask, and passthrough. For a description of
these values, see ACL Properties.</source>
        <translation>नियंत्रित करता है के कैसे एक एसीएल प्रविष्टि एक सीएचमोड आपरेशन के दौरान संशोधित किया गया है
त्यागना, समूह मास्क, और पासथ्रू इसके मूल्य हैं | इसके वर्णन के लिए
इन मूल्यों के, एसीएल प्रॉपर्टीस देखें |
       </translation>
    </message>
    <message>
        <source>Controls whether the access time for files is updated when they are read.
Turning this property off avoids producing write traffic when reading files and
can result in significant performance gains, though it might confuse mailers and
similar utilities.</source>
        <translation>नियंत्रित करता है के जब वे पढ़ रहे होते हैं, क्या तब फ़ाइलों के लिए उपयोग समय अद्यतन किया जाता है
इस संपत्ति को बंद करने से, जब फाइलें पढ़ी जा रही होंगी तब फाइल लिखते वक़्त यातायात के निर्माण से बचेगी और
उल्लेखनीय प्रदर्शन लाभ में परिणाम कर सकते हैं, हालांकि यह मेलर्स को भ्रमित कर सकता है और
इसी तरह की उपयोगितायें
            </translation>
    </message>
    <message>
        <source>Read-only property that identifies the amount of disk space available to a dataset
and all its children, assuming no other activity in the pool. Because disk space is
shared within a pool, available space can be limited by various factors including
physical pool size, quotas, reservations, and other datasets within the pool.</source>
        <translation>रीड ओन्ली प्रॉपर्टी, जो डाटा सेट के लिए उपलब्ध डिस्क स्थान की मात्रा को पहचानती है
और उसके सब बच्चे, समझते हुए के पूल में और कोई गतिविधि नहीं चल रही | क्योंकि डिस्क स्थान है
एक पूल में साझा, उपलब्ध स्थान विभिन्न कारणों द्वारा सीमित किया जा सकता है
शारीरिक पूल आकार, कोटा, आरक्षण, और पूल के भीतर अन्य डेटासेट
            </translation>
    </message>
    <message>
        <source>If this property is set to off, the file system cannot be mounted,
and is ignored by &apos;zfs mount -a&apos;.  Setting this property to off is
similar to setting the mountpoint property to none, except that the
dataset still has a normal mountpoint property, which can be inherited
Setting this property to off allows datasets to be used solely as a
mechanism to inherit properties. One example of setting canmount=off is
to have two datasets with the same mountpoint, so that the children of
both datasets appear in the same directory, but might have different
inherited characteristics.</source>
        <translation>अगर यह प्रॉपर्टी बंद कर दी जाती है, तो फाइल सिस्टम माउंट नहीं किया जा सकेगा,
और &apos;ज़ीएफएस माउंट-ए&apos; द्वारा नजरअंदाज कर दिया गया है | इस प्रॉपर्टी को बंद करना
माउंटपॉइंट प्रॉपर्टी को कोई नहीं करने पे सेट करने के बराबर है, सिवाय इसके कि
डेटासेट का अभी भी सामान्य माउंट पॉइंट प्रॉपर्टी है, जो विरासत से लिया जा सकता है
इस प्रॉपर्टी को बंद करना, डेटासेट को केवल एक के रूप में इस्तेमाल करने की अनुमति देता
प्रॉपर्टी को विरासत में पाने का तंत्र है | एक उदाहरण जिसमे माउंट हो सकता है=बंद है सेट किया है
एक ही माउंट पॉइंट से दो डेटा सेट होना, ताकि बच्चे
दोनो डेटा सेट के, एक ही निर्देशिका में दिखाई दें, पर शाएद अलग
विरासत में मिले विशेषताएँ हों |
</translation>
    </message>
    <message>
        <source>Controls the checksum used to verify data integrity. The default
value is on, which automatically selects an appropriate algorithm
(currently, fletcher4, but this may change in future releases). The
value off disables integrity checking on user data. Disabling
checksums is NOT a recommended practice.</source>
        <translation>डेटा अखंडता को सत्यापित करने के लिए इस्तेमाल किया चेकसम नियंत्रित करता है | डिफॉल्ट
मूल्य ऑन है, जो स्वतः ही एक उपयुक्त कलन विधि का चयन करता है
(वर्तमान में, फ्लेचर4, लेकिन यह भविष्य के रिलीज में बदल सकते हैं) | 
मूल्य से उपयोगकर्ता डेटा पर जाँच अखंडता को निष्क्रिय करता ह | असक्षम
चेकसम कोई अनुशंसित अभ्यास नहीं है
</translation>
    </message>
    <message>
        <source>Enables or disables compression for a dataset. The values are on, off, lzjb, zle, gzip,
and gzip-N. Currently, setting this property to lzjb, gzip, or gzip-N has the same
effect as setting this property to on. Enabling compression on a file system with
existing data only compresses new data. Existing data remains uncompressed.</source>
        <translation>सक्षम बनाता है या एक डाटासेट के लिए संपीड़न को निष्क्रिय करता है | मूल्य हैं, ऑन, ऑफ, एलज़ीजेबी, ज़ीएलई, गीज़िप,
और जीज़िप-एन | वर्तमान में, इस प्रॉपर्टी को एलज़ीजेबी,गीज़िप और जीज़िप-एन में सेट करने से हूबहू
प्रभाव आएँगे, जैसे इस प्रॉपर्टी को ऑन पे सेट करके आएँगे | साथ ही एक फाइल सिस्टम पर संपीड़न सक्षम करने से
मौजूदा डेटा केवल नए डेटा संपीड़न करेगा | मौजूदा डेटा असम्पीडित बना रहेगा |
</translation>
    </message>
    <message>
        <source>Read-only property that identifies the compression ratio achieved for a dataset,
expressed as a multiplier. Compression can be enabled by the zfs set compression=on
dataset command. The value is calculated from the logical size of all files and the
amount of referenced physical data. It includes explicit savings through the use of
the compression property.</source>
        <translation>रीड ओन्ली प्रॉपर्टी जो दिखाती है के संपीड़न अनुपात कितना हासिल हुआ एक डाटासेट के लिए,
एक गुणक के रूप में व्यक्त किया | संपीड़न =ऑन पर सम्पीडन सेट ज़ीएफएस रूप से सक्षम किया जा सकता है
डाटासेट आदेश | मूल्य की सभी फाइलों की तार्किक आकार से गणना की है और
संदर्भित शारीरिक डेटा की राशि | इसमे स्पष्ट बचत शामिल है, इसके उपयोग के माध्यम से
संपीड़न प्रॉपर्टी
</translation>
    </message>
    <message>
        <source>Sets the number of copies of user data per file system. These copies
are in addition to any pool-level redundancy. Disk space used by
multiple copies of user data is charged to the corresponding file and
dataset, and counts against quotas and reservations. In addition, the
used property is updated when multiple copies are enabled. Consider setting
this property when the file system is created because changing this property
on an existing file system only affects newly written data.</source>
        <translation>फाइल सिस्टम प्रति उपयोगकर्ता डेटा की प्रतियों की संख्या सेट करता है | यह कॉपियाँ
किसी भी पूल स्तर के अतिरेक के अतिरिक्त हैं | डिस्क स्थान प्रयुक्त द्वारा
उपयोगकर्ता डेटा की कई प्रतियां तदनुसार फाइल से प्रभारित है और
डाटासेट, और कोटा और आरक्षण के खिलाफ मायने रखता है | इसके अतिरिक्त
इस्तेमाल की गई प्रॉपर्टी को अद्यतन किया जाता है जब कई प्रतियां सक्षम हों | सेट्टिंग्स को ध्यान में रखें
जब फाइल सिस्टम का निर्माण होता है, क्योंकि इस प्रॉपर्टी को बदलने से 
एक मौजूदा फाइल सिस्टम पर, केवल नये लिखे हुए डेटा को प्रभावित करता है
</translation>
    </message>
    <message>
        <source>Read-only property that identifies the date and time that a dataset was created.</source>
        <translation>रीड ओन्ली प्रॉपर्टी वो तिथि और वक़्त बताती है, जब डेटासेट का निर्माण हुआ था
            </translation>
    </message>
    <message>
        <source>Configures deduplication for a dataset. The default value is off.
The default deduplication checksum is sha256 (this may change in the
future).  When dedup is enabled, the checksum defined here overrides
the checksum property. Setting the value to verify has the same
effect as the setting sha256,verify.
If set to verify, ZFS will do a byte-to-byte comparsion in case of
two blocks having the same signature to make sure the block contents
are identical.</source>
        <translation>एक डाटासेट के लिए द्विरावृत्ति को हटाने को समनुरूप करता है | डिफ़ॉल्ट मूल्य ऑफ है
डिफ़ॉल्ट द्विरावृत्ति को हटाने वाला चेकसम शा256 है (ये बदल सकता है
भविष्य में) | जब द्विरावृत्ति को हटाना सक्षम होता है, तो यहाँ परिभाषित चेकसम अधिभूत होता है
चेकसम प्रॉपर्टी में | मूल्य को सेट करने के लिए सत्यापित करने के लिए
एक ही प्रभाव है जैसे शा256 सेट करना, पुष्टि करें | 
अगर सत्यापित करने के लिए सेट करते हैं, ज़ीएफएस बाइट दर बाइट तुलना करेगा, उस मामले में
दो ब्लॉक के एक जैसे हस्ताक्षर हो, सुनिश्चित करने के लिए के ब्लॉक सामग्री
समान हैं |
</translation>
    </message>
    <message>
        <source>The devices property is currently not supported on FreeBSD.</source>
        <translation>उपकर्ण की प्रॉपर्टी वर्तमान में फ्री बीएसडी को समर्थित नहीं करती
            </translation>
    </message>
    <message>
        <source>Controls whether processes can be executed from within this file
system. The default value is on.</source>
        <translation>नियंत्रण करता है के क्या क्रियायें फाइल के भीतर से क्रियान्वित किया जा सकती है
सिस्टम | डिफॉल्ट मूल्य ऑन है
            </translation>
    </message>
    <message>
        <source>Read-only property that indicates whether a file system, clone, or snapshot
is currently mounted.</source>
        <translation>रीड ओन्ली प्रॉपर्टी जो बताता है के क्या फाइल सिस्टम, क्लोन, या स्नेपशॉट
वर्तमान में माउंट हो चुकी है |
            </translation>
    </message>
    <message>
        <source>Controls the mount point used for this file system. When the mountpoint property
is changed for a file system, the file system and any descendents that inherit the
mount point are unmounted. If the new value is legacy, then they remain unmounted.
Otherwise, they are automatically remounted in the new location if the property was
previously legacy or none, or if they were mounted before the property was changed.
In addition, any shared file systems are unshared and shared in the new location.</source>
        <translation>इस फाइल सिस्टम के लिए इस्तेमाल किया माउंट पॉइंट नियंत्रण करता है | जब माउंट पॉइंट प्रॉपर्टी
एक फाइल सिस्टम के लिए बदली जाती है, फाइल सिस्टम और कोई भी वंशज जो विरासत में पाता है
वो माउंट पॉइंट, अनमाउंट होते हैं | अगर नया मूल्य विरासत का है, तो वो अनमाउंट रहता है |
अन्यथा, वे स्वचालित रूप से नए स्थान में दोबारा माउंट हो जाते हैं, अगर वो प्रॉपर्टी
पहले से विरासत या कोई नहीं, या वह प्रॉपर्टी बदलने से पहले माउंट होती है | 
इसके अलावा, कोई भी साझा फाइल सिस्टम, नये स्थान पे अविभाजित और सांझे होते हैं |
            </translation>
    </message>
    <message>
        <source>Controls what is cached in the primary cache (ARC). Possible values are all,
none, and metadata. If set to all, both user data and metadata are cached. If
set to none, neither user data nor metadata is cached. If set to metadata, only
metadata is cached.</source>
        <translation>नियंत्रित करता है के प्राथमिक कैश (एआरसी) में कैश क्या है | संभव मूल्य हैं, सब
कोई नहीं और मेटाडेटा | अगर सभी पे सेट किया जाए, तो दोनो, उपयोगकर्ता डेटा और मेटाडेटा कैश हो जाते हैं | अगर
कोई नहीं पर सेट करते हैं, तो ना तो उपयोगकर्ता डेटा और न ही मेटाडाटा कैश होता है | अगर मेटाडेटा पे सेट करते हैं,तो सिर्फ़
मेटाडाटा कैश होता है |
            </translation>
    </message>
    <message>
        <source>Read-only property for cloned file systems or volumes that identifies
the snapshot from which the clone was created. The origin cannot be
destroyed (even with the -r or -f option) as long as a clone exists.
Non-cloned file systems have an origin of none.</source>
        <translation>रीड-ओन्ली प्रॉपर्टी क्लोन किए फाइल सिस्टम के लिए या संस्करणों के लिए जो दिखाते हैं
स्नेपशॉट जहाँ से क्लोन निर्मित किया गया | मूल नहीं हो सकता
नष्ट (यहां तक ​​कि -आर या -एफ विकल्प के साथ) जब तक क्लोन मौजूद है |
गैर क्लोन फाइल सिस्टम से कोई भी एक मूल नहीं है |
</translation>
    </message>
    <message>
        <source>Limits the amount of disk space a dataset and its descendents can consume.
This property enforces a hard limit on the amount of disk space used,
including all space consumed by descendents, such as file systems and snapshots.
Setting a quota on a descendent of a dataset that already has a quota does not
override the ancestor&apos;s quota, but rather imposes an additional limit.</source>
        <translation>डिस्क स्थान की मात्रा की सीमा निर्धारित करता है जो एक डाटासेट और उसके वंशजों का उपभोग कर सकते हैं
यह प्रॉपर्टी प्रयुक्त डिस्क स्थान की मात्रा पर एक मुश्किल सीमा लागू करता है,
वंशजों द्वारा खपत की गई सभी स्थान सहित, जैसे के फाइल सिस्टम और स्नेपशॉट |
डेटा सेट के वंशज पे कोटा सेट करना, जिसपे पहले से हे कोटा है,
पूर्वज कोटा पर हावी नहीं होता, बल्कि एक अतिरिक्त सीमा लगाता है |
</translation>
    </message>
    <message>
        <source>Controls whether a dataset can be modified. When set to on,
no modifications can be made.</source>
        <translation>नियंत्रित करता है कि क्या एक डाटासेट संशोधित किया जा सकता है | जब ऑन पे सेट करें,
कोई संशोधन नहीं कर सकते |
</translation>
    </message>
    <message>
        <source>Specifies a suggested block size for files in the file system. This
property is designed solely for use with database workloads that
access files in fixed-size records.  ZFS automatically tunes block
sizes according to internal algorithms optimized for typical access
patterns.
Use of this property for general purpose file systems is strongly
discouraged, and may adversely affect performance.
The size specified must be a power of two greater than or equal to
512 and less than or equal to 128 Kbytes.</source>
        <translation>फाइल सिस्टम में फ़ाइलों के लिए निर्दिष्ट ब्लॉक आकार का सुझाव दिया करता है | यह
प्रॉपर्टी सिर्फ़ और सिर्फ़ डेटाबेस वर्कलोड के साथ प्रयोग के लिए बनाया गया है, जो
निश्चित आकार के रिकॉर्ड में फाइलें को प्रवेश करवाता है | ज़ीएफएस स्वतः ब्लॉक को मिलाता है
आकार अनुकूलित आंतरिक एल्गोरिदम के अनुसार उपयोग करने के लिए ठेठ
स्वरूप |
इस प्रॉपर्टी का प्रयोग सामान्य प्रयोजन फाइल सिस्टम के लिए दृढ़ता से
हतोत्साहित है, और प्रतिकूल प्रदर्शन को प्रभावित कर सकता है |
निर्दिष्ट आकार एक से दो अधिक के अधिपति या बराबर होना चाहिए
512 और कम से कम या 128 कीलो बाइट्स के बराबर
</translation>
    </message>
    <message>
        <source>Read-only property that identifies the amount of data accessible by a
dataset, which might or might not be shared with other datasets in the
pool.</source>
        <translation>रीड ओन्ली प्रॉपर्टी बताता है, सुलभ डेटा की राशि
एक डाटासेट से, जो बाकी डेटासेट के साथ, सांझा हो सकता है या नहीं भी हो सकता है
पूल में
         </translation>
    </message>
    <message>
        <source>Sets the amount of disk space that a dataset can consume. This property
enforces a hard limit on the amount of space used. This hard limit does
not include disk space used by descendents, such as snapshots and clones.</source>
        <translation>डिस्क की जगह की मात्रा जो एक डेटा सेट इस्तेमाल कर सकता है, उसको सेट करता है | यह प्रॉपर्टी
इस्तेमाल जगह की मात्रा पर एक मुश्किल सीमा लागू करता है | यह मुश्किल सीमा
में, डिस्क की जगह जो के वंशज इस्तेमाल करते हैं, जैसे के स्नेपशॉट और क्लोन, शामिल नहीं है |
</translation>
    </message>
    <message>
        <source>Sets the minimum amount of disk space that is guaranteed to a dataset, not
including descendents, such as snapshots and clones. When the amount of disk
space used is below this value, the dataset is treated as if it were taking up
the amount of space specified by refreservation. The refreservation reservation
is accounted for in the parent dataset&apos;s disk space used, and counts against
the parent dataset&apos;s quotas and reservations.
If refreservation is set, a snapshot is only allowed if enough free pool space
is available outside of this reservation to accommodate the current number of
referenced bytes in the dataset.</source>
        <translation>एक डाटासेट की गारंटी है कि डिस्क स्थान की न्यूनतम मात्रा सेट करता है, नहीं
जिसमे शामिल है, वंशज, जैसे के स्नेपशॉट और क्लोन | जब डिस्क की मात्रा
इस्तेमाल की गई जगह इस मूल्य से कम है, डेटा सेट के साथ ऐसा व्यवहार किया जाता है, मानो इसे उपर लेजाया जा रहा हो
रेफरिज़र्वेशन द्वारा निर्दिष्ट स्थान की मात्रा | रेफरिज़र्वेशन रिज़र्वेशन
इस्तेमाल किया पेरेंट डाटासेट के डिस्क स्थान के लिए जिम्मेदार है, और उसके प्रतिकूल
पेरेंट डाटासेट के कोटा और आरक्षण के |
अगर रिज़र्वेशन सेट की जाती है, एक स्नैपशॉट को तभी अनुमति मिलती है, अगर पर्याप्त फ्री पूल जगह
वर्तमान संख्या को समायोजित करने के लिए इस आरक्षण के बाहर उपलब्ध है
डाटासेट में संदर्भित बाइट्स |
</translation>
    </message>
    <message>
        <source>Sets the minimum amount of disk space guaranteed to a dataset and its descendents.
When the amount of disk space used is below this value, the dataset is treated as if
it were using the amount of space specified by its reservation. Reservations are
accounted for in the parent dataset&apos;s disk space used, and count against the parent
dataset&apos;s quotas and reservations.</source>
        <translation>एक डाटासेट और अपने वंशजों के लिए गारंटी डिस्क स्थान की न्यूनतम राशि तय करता है |
प्रयुक्त डिस्क स्थान की मात्रा इस मूल्य से कम होता है तो, डाटासेट के साथ ऐसा व्यवहार किया जाता है जैसे
यह अपने आरक्षण द्वारा निर्दिष्ट स्थान की मात्रा का उपयोग कर रहे थे | आरक्षण
इस्तेमाल हुई पेरेंट डेटा सेट डिस्क के स्थान के लिए जिम्मेदार होती है, और पेरेंट के अतिरिक्त
डाटासेट के कोटा और आरक्षण समझी जाती है
</translation>
    </message>
    <message>
        <source>Controls what is cached in the secondary cache (L2ARC). Possible values are all, none,
and metadata. If set to all, both user data and metadata are cached. If set to none,
neither user data nor metadata is cached. If set to metadata, only metadata is cached.</source>
        <translation>माध्यमिक कैश (एल2आर्क) में कैश क्या है, यह नियंत्रित करता है | संभव मूल्य हैं, सब, कोई नहीं
और मेटाडाटा | अगर सब पे सेट कर दिया जाए, उपयोगकर्ता डेटा और मेटाडेटा दोनों कैश में चले जाते हैं | अगर कोई नहीं पे सेट करते हैं
न तो उपयोगकर्ता डेटा और न ही मेटाडाटा कैश में जाता है | अगर मेटाडाटा पे सेट करते हैं, तो केवल मेटाडाटा कैश होता है |
</translation>
    </message>
    <message>
        <source>Controls whether the setuid bit is honored in a file system.</source>
        <translation>नियंत्रित करता है के क्या सेटयूआईडी बिट एक फाइल सिस्टम में सम्मानित किया गया है
            </translation>
    </message>
    <message>
        <source>Controls whether a file system is available over NFS and what
options are used. If set to on, the zfs share command is invoked
with no options. Otherwise, the zfs share command is invoked with
options equivalent to the contents of this property.</source>
        <translation>नियंत्रण करता है के क्या एक फाइल सिस्टम एनएफएस पर उपलब्ध है और क्या
विकल्प इस्तेमाल किए गये हैं | अगर ऑन पे सेट करें तो, ज़ीएफएस शेयर आदेश लागू होता है
बिना किसी विकल्प के | अन्यथा, ज़ी एफ एस शेयर आदेश लागू होता है
इस प्रॉपर्टी की सामग्री के बराबर विकल्पों के साथ. |
      </translation>
    </message>
    <message>
        <source>The sharesmb property has currently no effect on FreeBSD.</source>
        <translation>शेअरएमबी प्रॉपर्टी का वर्तमान में फ्री बीएसडी पर कोई प्रभाव नहीं है
            </translation>
    </message>
    <message>
        <source>Controls whether the .zfs directory is hidden or visible in the root of the file system.</source>
        <translation>नियंत्रण करता है के क्या ज़ीएफएस निर्देशिका, फाइल सिस्टम की रूट में छुपा या दिख रहा है
            </translation>
    </message>
    <message>
        <source>Read-only property that identifies the dataset type as filesystem (file system or clone), volume, or snapshot.</source>
        <translation>रीड ओन्ली प्रॉपर्टी जो दिखाती है डाटासेट प्रकार को फाइल सिस्टम(फाइल सिस्टम या क्लोन), संस्करण या स्नेपशॉट की तरह
            </translation>
    </message>
    <message>
        <source>Read-only property that identifies the amount of disk space consumed by a dataset and all its descendents.</source>
        <translation>रीड ओन्ली प्रॉपर्टी जो दिखाती है एक डाटासेट और उसके सभी वंशजों द्वारा खपत डिस्क स्थान की मात्रा
            </translation>
    </message>
    <message>
        <source>Read-only property that identifies the amount of disk space that is used by
children of this dataset, which would be freed if all the dataset&apos;s children
were destroyed.</source>
        <translation>रीड ओन्ली प्रॉपर्टी जो दिखाती है उपयोग की जाने वाली डिस्क स्थान की मात्रा
इस डाटासेट के बच्चों द्वारा, जो मुक्त किया जाएगा अगर सभी डाटासेट के बच्चों को
नष्ट किया जाए
</translation>
    </message>
    <message>
        <source>Read-only property that identifies the amount of disk space that is used by
a dataset itself, which would be freed if the dataset was destroyed, after
first destroying any snapshots and removing any refreservation reservations.</source>
        <translation>रीड ओन्ली प्रॉपर्टी जो दिखाती है उपयोग की जाने वाली डिस्क स्थान की मात्रा
एक डाटासेट से ही, जो मुक्त किया जाएगा अगर डाटासेट नष्ट हो गया, बाद में
पहले किसी भी स्नेपशॉट को नष्ट करने और किसी भी रेफरिज़र्वेशन आरक्षण को हटाने में
         </translation>
    </message>
    <message>
        <source>Read-only property that identifies the amount of disk space that is used by a
refreservation set on a dataset, which would be freed if the refreservation was
removed.</source>
        <translation>रीड ओन्ली प्रॉपर्टी जो दिखाती है उपयोग की जाने वाली डिस्क स्थान की मात्रा
रेफरिज़र्वेशन  एक डाटासेट पर सेट होने पे, जो मुक्त किया जाएगा अगर रेफरिज़र्वेशन
हटा दिया जाता
</translation>
    </message>
    <message>
        <source>Read-only property that identifies the amount of disk space that is consumed
by snapshots of a dataset. In particular, it is the amount of disk space that
would be freed if all of this dataset&apos;s snapshots were destroyed. Note that
this value is not simply the sum of the snapshots&apos; used properties, because
space can be shared by multiple snapshots.</source>
        <translation>रीड ओन्ली प्रॉपर्टी जो दिखाती है डिस्क स्थान की मात्रा जो उपभोग की हो
एक डाटासेट की स्नेपशॉट से | विशेष रूप से, यह वह डिस्क स्थान की मात्रा है जो कि
मुक्त हो जाएगा अगर इस डाटासेट के  सभी स्नेपशॉट को नष्ट कर दिया जाए | नोट करें
यह मूल्य केवल स्नेपशॉट &apos;का इस्तेमाल किया गुणों का योग नहीं है, क्यूंकी
जगह एकाधिक स्नेपशॉट द्वारा साझा की जा सकती है |
     </translation>
    </message>
    <message>
        <source>Identifies the on-disk version of a file system, which is independent of the
pool version. This property can only be set to a later version that is available
from the supported software release.</source>
        <translation>एक फाइल सिस्टम के ऑन डिस्क संस्करण को दिखाता है,जो स्वतंत्र है
पूल संस्करण से | यह प्रॉपर्टी केवल बाद के संस्करण के लिए सेट हो सकती है, जो उपलब्ध होगा
समर्थित सॉफ्टवेयर रिलीज से
</translation>
    </message>
    <message>
        <source>For volumes, specifies the logical size of the volume.</source>
        <translation>संस्करण के लिए, संस्करण के तार्किक आकार को निर्दिष्ट करता है
            </translation>
    </message>
    <message>
        <source>For volumes, specifies the block size of the volume.</source>
        <translation>संस्करण के लिए, संस्करण के ब्लॉक साइज़ को निर्दिष्ट करता है
            </translation>
    </message>
    <message>
        <source>Indicates whether a dataset has been added to a jail.</source>
        <translation>दिखाता है के क्या एक डाटासेट जेल में जोड़ा गया है
            </translation>
    </message>
    <message>
        <source>This property is not supported on FreeBSD.</source>
        <translation>यह प्रॉपर्टी फ्री बीएसडी पर समर्थित नहीं है
            </translation>
    </message>
    <message>
        <source>The file system is marked to be destroyed after unmount.</source>
        <translation>चिह्नित है के फाइल सिस्टम को अनमाउंट के बाद नष्ट कर दिया जाएगा
            </translation>
    </message>
    <message>
        <source>The compression ratio achieved for the referenced space of this
dataset, expressed as a multiplier.</source>
        <translation>संदर्भित स्थान के लिए हासिल संपीड़न अनुपात
डाटासेट, एक गुणक के रूप में व्यक्त किया |
            </translation>
    </message>
    <message>
        <source>The amount of referenced space written to this dataset since the
previous snapshot.</source>
        <translation>के बाद से इस डाटासेट के लिए लिखा संदर्भित अंतरिक्ष की मात्रा
पिछले स्नैपशॉट
</translation>
    </message>
    <message>
        <source>This property is set to the number of user holds on this snapshot.
</source>
        <translation>इस प्रॉपर्टी को सेट किया गया है, उपयोगकर्ता की संख्या से, इस स्नेपशॉट पे
            </translation>
    </message>
    <message>
        <source>Provide a hint to ZFS about handling of synchronous requests in this
dataset.  If logbias is set to latency (the default), ZFS will use
pool log devices (if configured) to handle the requests at low
latency. If logbias is set to throughput, ZFS will not use configured
pool log devices.  ZFS will instead optimize synchronous operations
for global pool throughput and efficient use of resources.</source>
        <translation>इस में तुल्यकालिक अनुरोध की हैंडलिंग के बारे में ज़ीएफएस करने के लिए एक संकेत मिलता है
डाटासेट | अगर लॉगबाइयस विलंबता पर सेट करते हैं (डिफॉल्ट), ज़ीएफएस इस्तेमाल करेगा
पूल लॉग उपकर्ण (अगर समनुरूप करें) कम से कम अनुरोधों को हैंडल करने के लिए
विलंबता में | अगर लॉगबाइयस थ्रौपुट पे सेट करें, ज़ीएफएस समनुरूप नहीं होगा
पूल लॉग उपकर्ण | इसके बजाय ज़ीएफएस तुल्यकालिक आपरेशनों को अनुकूलित करेंगे
वैश्विक पूल थ्रौपुट और संसाधनों के कुशल उपयोग के लिए |
</translation>
    </message>
    <message>
        <source>Controls the behavior of synchronous requests (e.g.  fsync(2),
O_DSYNC). This property accepts the following values:
standard  This is the POSIX specified behavior of ensuring all
          synchronous requests are written to stable storage and
          all devices are flushed to ensure data is not cached by
          device controllers (this is the default).
always    All file system transactions are written and flushed
          before their system calls return. This has a large
          performance penalty.
disabled  Disables synchronous requests. File system transactions
          are only committed to stable storage periodically. This
          option will give the highest performance.  However, it
          is very dangerous as ZFS would be ignoring the synchro-
          nous transaction demands of applications such as data-
          bases or NFS.  Administrators should only use this
          option when the risks are understood.</source>
        <translation>तुल्यकालिक अनुरोधों के व्यवहार को नियंत्रित करता है (जैसे के, एफसिंक(2),
ओ_डीसिंक) | यह प्रॉपर्टी निम्न मूल्य स्वीकार करती है:
मानक   यह पॉज़िक्स निर्दिष्ट व्यवहार है यह सुनिश्चित करने के लिए के सभी
                                         तुल्यकालिक अनुरोध को स्थिर भंडारण के लिए लिखा जाता है और
                                         सभी उपकरणों को प्लावित किया जाता है, यह सुनिश्चित करने के लिए के डेटा कैश नहीं होता
                                         उपकरण नियंत्रकों के ज़रिए(यह डिफॉल्ट है) |
हमेशा     सभी फाइल सिस्टम लेनदेन लिखा और प्लावित किया जाता है
                                         अपने सिस्टम कॉल वापसी से पहले | उनके पास एक लंबा
                                         प्रदर्शन जुर्माना है |
असक्षम   तुल्यकालिक अनुरोध को निष्क्रिय करता है | फाइल सिस्टम लेनदेन
                                         केवल समय - समय स्थिर भंडारण के लिए प्रतिबद्ध हैं | यह
                                         विकल्प उच्चतम प्रदर्शन देगा | तथापि
                                         यह बहुत खतरनाक है क्यूकी ज़ीएफएस तुल्यकालिक लेन - देन मांगों की
                                         अनदेखी कर रहा होगा उन अप्लिकेशन की, जैसे के
                                         डेटाबेस या एनएफएस | व्यवस्थापकों को तभी इस्तेमाल करना चाहिए
                                         इस विकल्प को जब जोखिम समझ आ गये हों
</translation>
    </message>
    <message>
        <source>This property cannot be changed after the system is created.</source>
        <translation>सिस्टम बनाए जाने के बाद इस प्रॉपर्टी को बदला नहीं जा सकता
            </translation>
    </message>
    <message>
        <source>User Property</source>
        <translation>उपयोगकर्ता प्रॉपर्टी
            </translation>
    </message>
    <message>
        <source>Unknown property or information not available.
Read ZFS manual for more information.</source>
        <translation>अज्ञात प्रॉपर्टी या जानकारी उपलब्ध नहीं
अधिक जानकारी के लिए ज़ीएफएस मैनुअल पढ़ें
</translation>
    </message>
    <message>
        <source>[Invalid]</source>
        <translation>[अमान्य]
            </translation>
    </message>
    <message>
        <source>[Inherit]</source>
        <translation>[उत्तराधिकार]
            </translation>
    </message>
    <message>
        <source>[Added]</source>
        <translation>[जोड़ा गया]
            </translation>
    </message>
    <message>
        <source>[Changed]</source>
        <translation>[बदला गया]
            </translation>
    </message>
    <message>
        <source>User property</source>
        <translation>उपयोगकर्ता प्रॉपर्टी
            </translation>
    </message>
    <message>
        <source>Device Menu</source>
        <translation>उपकर्ण मेनू
            </translation>
    </message>
    <message>
        <source>Add new user property</source>
        <translation>नये उपयोगकर्ता प्रॉपर्टी को जोड़ें
            </translation>
    </message>
    <message>
        <source>Delete user property</source>
        <translation>उपयोगकर्ता प्रॉपर्टी हटायें
            </translation>
    </message>
</context>
<context>
    <name>DialogMount</name>
    <message>
        <source>Dialog</source>
        <translation>संवाद
            </translation>
    </message>
    <message>
        <source>Device to mount:</source>
        <translation>माउंट करने के लिए उपकर्ण:
            </translation>
    </message>
    <message>
        <source>(Device)</source>
        <translation>(उपकर्ण)
            </translation>
    </message>
    <message>
        <source>Mount location:</source>
        <translation>माउंट करने का स्थान:
            </translation>
    </message>
    <message>
        <source>...</source>
        <translation>...
            </translation>
    </message>
    <message>
        <source>Select Directory</source>
        <translation>निर्देशिका चुनें
            </translation>
    </message>
</context>
<context>
    <name>DialogName</name>
    <message>
        <source>Dialog</source>
        <translation>संवाद
            </translation>
    </message>
    <message>
        <source>Pool name:</source>
        <translation>पूल का नाम:
            </translation>
    </message>
    <message>
        <source>Do not mount any filesystems</source>
        <translation>किसी फाइल सिस्टम को माउंट ना करें
            </translation>
    </message>
    <message>
        <source>Read only pool</source>
        <translation>रीड ओन्ली पूल
            </translation>
    </message>
    <message>
        <source>Import options:</source>
        <translation>विकल्प आयात करें:
            </translation>
    </message>
    <message>
        <source>Mount with alternative root:</source>
        <translation>वैकल्पिक रूट के साथ माउंट करें:
            </translation>
    </message>
    <message>
        <source>Mount all filesystems automatically</source>
        <translation>स्वचालित रूप से सभी फाइल सिस्टम माउंट करें
            </translation>
    </message>
    <message>
        <source>Mounting behavior:</source>
        <translation>माउंट होने का व्यवहार:
            </translation>
    </message>
    <message>
        <source>Force operation if error (use with caution)</source>
        <translation>ऑपरेशन पे बल डालें, अगर त्रुटि हो (सावधानी के साथ प्रयोग करें)
            </translation>
    </message>
</context>
<context>
    <name>DialogNewPool</name>
    <message>
        <source>Dialog</source>
        <translation>संवाद
            </translation>
    </message>
    <message>
        <source>Selected</source>
        <translation>चयनित
            </translation>
    </message>
    <message>
        <source>Pool name:</source>
        <translation>पूल का नाम:
            </translation>
    </message>
    <message>
        <source>Configuration type:</source>
        <translation>समाकृति प्रकार:
            </translation>
    </message>
    <message>
        <source>Striped</source>
        <translation>धारीदार
            </translation>
    </message>
    <message>
        <source>Mirror</source>
        <translation>दर्पण
            </translation>
    </message>
    <message>
        <source>Raid-Z (single parity)</source>
        <translation>रेड-ज़ी (एकल समता)
            </translation>
    </message>
    <message>
        <source>Raid-Z2 (double parity)</source>
        <translation>रेड-ज़ी2 (दोहरी समता)
            </translation>
    </message>
    <message>
        <source>Raid-Z3 (triple parity)</source>
        <translation>रेड-ज़ी3 (तिहरी समता)
            </translation>
    </message>
    <message>
        <source>Cache</source>
        <translation>कैश
            </translation>
    </message>
    <message>
        <source>Log</source>
        <translation>लॉग
            </translation>
    </message>
    <message>
        <source>Spare</source>
        <translation>अतिरिक्त
            </translation>
    </message>
    <message>
        <source>Error Description Here</source>
        <translation>त्रुटि विवरण यहाँ है
            </translation>
    </message>
    <message>
        <source>Must provide a valid pool name</source>
        <translation>एक वैध पूल नाम देना होगा
            </translation>
    </message>
    <message>
        <source>Must select exactly %1 devices</source>
        <translation>यथार्थतः %1 उपकरणों का चयन करना चाहिए
            </translation>
    </message>
    <message>
        <source>Must select at least one device</source>
        <translation>कम से कम एक उपकरण चुनना जरूरी
            </translation>
    </message>
</context>
<context>
    <name>DialogPartition</name>
    <message>
        <source>Add Partition</source>
        <translation>विभाजन जोड़ें
            </translation>
    </message>
    <message>
        <source>Add new slice/partition</source>
        <translation>नया टुकड़ा / विभाजन जोड़ें
            </translation>
    </message>
    <message>
        <source>100 GB</source>
        <translation>100 जीबी
            </translation>
    </message>
    <message>
        <source>Device:</source>
        <translation>उपकरण:
            </translation>
    </message>
    <message>
        <source>ada0</source>
        <translation>अदा0
            </translation>
    </message>
    <message>
        <source>Largest available free block:</source>
        <translation>सबसे बड़ा उपलब्ध फ्री ब्लॉक:
            </translation>
    </message>
    <message>
        <source>Total Size:</source>
        <translation>कुल साइज़:
            </translation>
    </message>
    <message>
        <source>Current layout:</source>
        <translation>वर्तमान लेआउट:
            </translation>
    </message>
    <message>
        <source>Designation</source>
        <translation>पदनाम
            </translation>
    </message>
    <message>
        <source>Start Sector</source>
        <translation>प्रारंभ का सेक्टर
            </translation>
    </message>
    <message>
        <source>Sector Count</source>
        <translation>सेक्टर गिनती
            </translation>
    </message>
    <message>
        <source>Size</source>
        <translation>साइज़
            </translation>
    </message>
    <message>
        <source>Color</source>
        <translation>रंग
            </translation>
    </message>
    <message>
        <source>New partition Size:</source>
        <translation>नया विभाजन साइज:
            </translation>
    </message>
    <message>
        <source>bytes</source>
        <translation>बाइट्स
            </translation>
    </message>
    <message>
        <source>kB</source>
        <translation>केबी
            </translation>
    </message>
    <message>
        <source>MB</source>
        <translation>एमबी
            </translation>
    </message>
    <message>
        <source>GB</source>
        <translation>जीबी
            </translation>
    </message>
    <message>
        <source>TB</source>
        <translation>टीबी
            </translation>
    </message>
    <message>
        <source>Partition type:</source>
        <translation>विभाजन प्रकार:
            </translation>
    </message>
    <message>
        <source>freebsd-zfs</source>
        <translation>फ्री बीएसडी-ज़ीएफएस
            </translation>
    </message>
    <message>
        <source>freebsd-ufs</source>
        <translation>फ्री बीएसडी-यूएफएस
            </translation>
    </message>
    <message>
        <source>freebsd</source>
        <translation>फ्री बीएसडी
            </translation>
    </message>
    <message>
        <source>freebsd-boot</source>
        <translation>फ्री बीएसडी-बूट
            </translation>
    </message>
    <message>
        <source>freebsd-swap</source>
        <translation>फ्री बीएसडी-विनिमय
            </translation>
    </message>
    <message>
        <source>bios-boot</source>
        <translation>बाइयोस-बूट
            </translation>
    </message>
    <message>
        <source>efi</source>
        <translation>ईएफआई
            </translation>
    </message>
    <message>
        <source>Create and initialize a new filesystem</source>
        <translation>एक नई फाइल सिस्टम को बनाएँ और प्रारंभ करें
            </translation>
    </message>
    <message>
        <source>** FREE **</source>
        <translation>** फ्री **
            </translation>
    </message>
    <message>
        <source>No free space!</source>
        <translation>कोई खाली जगह नहीं!
            </translation>
    </message>
</context>
<context>
    <name>DialogProp</name>
    <message>
        <source>Edit Pool Properties</source>
        <translation>पूल प्रॉपर्टीस संपादित करें
            </translation>
    </message>
    <message>
        <source>Property</source>
        <translation>प्रॉपर्टी
            </translation>
    </message>
    <message>
        <source>Value</source>
        <translation>मूल्य
            </translation>
    </message>
    <message>
        <source>Total size of the storage pool.</source>
        <translation>भंडारण पूल का कुल आकार
            </translation>
    </message>
    <message>
        <source>Percentage of pool space used.</source>
        <translation>इस्तेमाल किया पूल स्थान का प्रतिशत
            </translation>
    </message>
    <message>
        <source>Alternate root directory. If set, this directory is prepended to any
mount points within the pool. This can be used when examining an
unknown pool where the mount points cannot be trusted, or in an
alternate boot environment, where the typical paths are not valid.
altroot is not a persistent property. It is valid only while the
system is up.  Setting altroot defaults to using cachefile=none, though
this may be overridden using an explicit setting.</source>
        <translation>वैकल्पिक रूट निर्देशिका | अगर सेट करें, इस निर्देशिका से किसी को सम्पन्न
पूल के भीतर माउंट पॉइंट होते हैं | यह इस्तेमाल किया जाता है, जब जाँच कर रहे हो
अज्ञात पूल, जहां माउंट पॉइंट पे भरोसा नहीं किया जा सकता है, या वहाँ
जहाँ वैकल्पिक बूट वातावरण हो, जहां ठेठ पथ मान्य नहीं हैं |
आल्टरूट एक लगातार प्रॉपर्टी नहीं है | यह वैध है ही, तभी
जब सिस्टम उठा हो | आल्टरूट डिफॉल्ट को कैशफाइल= कोई नहीं पे सेट करना, हलांकि
यह एक स्पष्ट सेटिंग का उपयोग करके अधिरोहित किया जा सकता है
</translation>
    </message>
    <message>
        <source>The current health of the pool.</source>
        <translation>पूल का वर्तमान स्वास्थ्य
            </translation>
    </message>
    <message>
        <source>A unique identifier for the pool.</source>
        <translation>पूल के लिए एक अद्वितीय पहचानकर्ता
</translation>
    </message>
    <message>
        <source>The current on-disk version of the pool. This can be increased, but
never decreased. The preferred method of updating pools is with the
&quot;zpool upgrade&quot; command, though this property can be used when a
specific version is needed for backwards compatibility. This property
can be any number between 1 and the current version reported by
&quot;zpool upgrade -v&quot;.</source>
        <translation>वर्तमान में पूल का ऑन डिस्क संस्करण | यह बढ़ाया जा सकता है, पर
कभी कम नहीं किया जा सकता | पूल अद्यतन करने के पसंदीदा विधि के साथ
&quot;ज़ीपूल उन्नयन&quot; कमांड, हालांकि इस प्रॉपर्टी का इस्तेमाल किया जा सकता है जब एक
पिछड़ी संगतता के लिए विशिष्ट संस्करण आवश्यक है | यह प्रॉपर्टी
कोई भी अंक हो सकता है, 1 और वर्तमान संस्करण के बीच, रिपोर्ट किया गया है
&quot;ज़ीपूल उन्नयन&quot; के ज़रिए |
</translation>
    </message>
    <message>
        <source>Identifies the default bootable dataset for the root pool. This
property is expected to be set mainly by the installation and upgrade
programs.</source>
        <translation>रूट पूल के लिए डिफॉल्ट बूटयोग्य डाटासेट पहचानता है | यह
प्रॉपर्टी की स्थापना और उन्नयन के द्वारा मुख्य रूप से निर्धारित कार्यक्रम उन्नयन
किए जाने की उम्मीद है
</translation>
    </message>
    <message>
        <source>Controls whether a non-privileged user is granted access based on the
dataset permissions defined on the dataset. See zfs(8) for more
information on ZFS delegated administration.</source>
        <translation>नियंत्रण करता है के क्या एक गैर विशेषाधिकार प्राप्त उपयोगकर्ता को प्रवेश प्रदान है, इस आधार पर
के डाटासेट अनुमतियों डाटासेट पर परिभाषित हों | अधिक जानकारी के लिए ज़ीएफएस(8)
ज़ीएफएस प्रत्यायोजित प्रशासन पर देखें
</translation>
    </message>
    <message>
        <source>Controls automatic device replacement. If set to &quot;off&quot;, device
replacement must be initiated by the administrator by using the
&quot;zpool replace&quot; command. If set to &quot;on&quot;, any new device, found in the
same physical location as a device that previously belonged to the
pool, is automatically formatted and replaced. The default behavior
is &quot;off&quot;.  This property can also be referred to by its shortened
column name, &quot;replace&quot;.</source>
        <translation>स्वचालित उपकरण प्रतिस्थापन नियंत्रित करता है | अगर &quot;ऑफ&quot; पे सेट किया जाए, उपकरण
प्रतिस्थापन व्यवस्थापक द्वारा शुरू किया जाना चाहिए, &quot;ज़ीपूल प्रतिस्थापन&quot; आदेश
का उपयोग करके | अगर &quot;ऑन&quot; पे सेट किया जाए, कोई भी नया उपकरण, जो मिले
वही भौतिक स्थान जैसे उपकरण के रूप में पहले
पूल का था, स्वचालित रूप से स्वरूपित और बदल दिया जाता है | डिफॉल्ट व्यवहार
&quot;ऑफ&quot; है | इस प्रॉपर्टी को भी जा सकता है, इसके आशुलिपि स्तंभ नाम
&quot;प्रतिस्थापन&quot; से कहा जा सकता है|
</translation>
    </message>
    <message>
        <source>Controls whether information about snapshots associated with this
pool is output when &quot;zfs list&quot; is run without the -t option. The
default value is off.</source>
        <translation>नियंत्रण करता है के क्या इस के साथ जुड़े स्नेपशॉट के बारे में जानकारी
पूल आउटपुट होता है, जब &quot;ज़ीएफएस सूची&quot; -टी विकल्प के बिना चलाई जाती है | इसका
डिफॉल्ट मूल्य ऑफ है |
</translation>
    </message>
    <message>
        <source>Controls automatic pool expansion when the underlying LUN is grown.
If set to &quot;on&quot;, the pool will be resized according to the size of the
expanded device. If the device is part of a mirror or raidz then all
devices within that mirror/raidz group must be expanded before the
new space is made available to the pool. The default behavior is
&quot;off&quot;.  This property can also be referred to by its shortened column
name, expand.</source>
        <translation>जब अंतर्निहित एलयूएन उगाया जाता है तब स्वत: पूल विस्तार नियंत्रित करता है |
अगर &quot;ऑन&quot; पे सेट करते हैं, पूल के आकार को विस्तार उपकर्ण हिसाब से बदल
दिया जाएगा | अगर उपकर्ण दर्पण या रेड्स का हिस्सा है तो सब
उस दर्पण / रेड्स समूह के बीच जो उपकर्ण हैं, उनका विस्तार किया जाना चाहिए, इससे पहले के
नयी जगह पूल के लिए उपलब्ध कराई जाए | डिफॉल्ट व्यवहार
&quot;ऑफ&quot; है | इस प्रॉपर्टी को भी, इसके आशुलिपि स्तंभ नाम
विस्तार से कहा जा सकता है |
</translation>
    </message>
    <message>
        <source>Threshold for the number of block ditto copies. If the reference
count for a deduplicated block increases above this number, a new
ditto copy of this block is automatically stored. Default setting is
0.</source>
        <translation>ब्लॉक दीट्टो कॉपीयों की संख्या के लिए सीमा | अगर
संदर्भ गिनती बिना अनुलिपि के लिए, एक संख्या के उपर बढ़ता है, तो एक नयी
इस ब्लॉक की दीट्टो कॉपी स्वचालित रूप से संग्रहीत की जाती है | डिफॉल्ट सेट्टिंग
0 है |
       </translation>
    </message>
    <message>
        <source>The deduplication ratio specified for a pool, expressed as a multiplier. For
example, a value of 1.76 indicates that 1.76 units of data were stored but
only 1 unit of disk space was actually consumed. See zfs(8) for a description
of the deduplication feature.</source>
        <translation>बिना अनुलिपि के एक पूल के लिए निर्दिष्ट अनुपात, एक गुणक के रूप में व्यक्त होती है | उदाहरण
के लिये, 1.76 का एक मूल्य दिखाता करता है कि डेटा की 1.76 इकाइयों संग्रहीत की गई पर
वास्तव में डिस्क स्थान का केवल 1 इकाई इस्तेमाल किया गया था | बिना अनुलिपि सुविधा का वर्णन के लिए
ज़ीएफएस (8) देखें |
</translation>
    </message>
    <message>
        <source>Number of blocks within the pool that are not allocated.</source>
        <translation>पूल के भीतर ब्लॉक की संख्या जो आवंटित नहीं की गई
            </translation>
    </message>
    <message>
        <source>Amount of storage space within the pool that has been physically allocated.</source>
        <translation>शारीरिक रूप से आवंटित की गई है पूल के भीतर भंडारण जगह की मात्रा
            </translation>
    </message>
    <message>
        <source>An arbitrary comment string set by the administrator.</source>
        <translation>व्यवस्थापक द्वारा सेट एक मनमाना टिप्पणी स्ट्रिंग
            </translation>
    </message>
    <message>
        <source>This property has currently no value on FreeBSD.</source>
        <translation>इस प्रॉपर्टी का वर्तमान में फ्री बीएसडी पर कोई मूल्य नहीं है
            </translation>
    </message>
    <message>
        <source>Controls the location of where the pool configuration is cached.
Discovering all pools on system startup requires a cached copy of the
configuration data that is stored on the root file system. All pools
in this cache are automatically imported when the system boots. Some
environments, such as install and clustering, need to cache this
information in a different location so that pools are not automatically
imported. Setting this property caches the pool configuration
in a different location that can later be imported with &quot;zpool import
-c&quot;.  Setting it to the special value &quot;none&quot; creates a temporary pool
that is never cached, and the special value &apos;&apos; (empty string) uses
the default location.</source>
        <translation>जहाँ पूल विन्यास कैश है उस स्थान को नियंत्रित करता है |
सिस्टम स्टार्टअप पर सभी पूल खोजने के लिए आवश्यकता है कैश्ड कॉपी की
जो रूट फाइल सिस्टम पर संग्रहीत है उस विन्यास डेटा की | सभी पूल
कैश में स्वचालित रूप से आयात कर दिए जाते हैं जब सिस्टम बूट होता है | कुछ
वातावरण, जैसे स्थापित करना और क्लस्टरिंग, कैश करने की जरूरत है
इस जानकारी को एक अलग स्थान में ताकि पूल स्वचालित रूप से नहीं 
आयात हों | इस प्रॉपर्टी को सेट करने से पूल विन्यास कैश होता है
एक अलग स्थान में जो कि बाद में &quot;ज़ीपूल आयात -सी&quot; &apos;के साथ आयात किया जा
सकता है | विशेष मूल्य &quot;कोई नहीं&quot; के लिए यह निर्धारित करना एक अस्थायी पूल बनाता है
जो कभी कैश्ड नहीं होता, और विशेष मूल्य &quot;कोई नहीं&quot; (खाली स्ट्रिंग) इस्तेमाल करता है
डिफॉल्ट स्थान को| 
            </translation>
    </message>
    <message>
        <source>If set to on, pool will be imported in read-only mode with the
following restrictions:
  *   Synchronous data in the intent log will not be accessible
  *   Properties of the pool can not be changed
  *   Datasets of this pool can only be mounted read-only
  *   To write to a read-only pool, a export and import of the pool
      is required.
</source>
        <translation>अगर ऑन पे सेट किया जाए, पूल केवल रीड ओन्ली मोड में आयात किया जाएगा
निम्नलिखित प्रतिबंध के साथ:
       *          आशय प्रवेश तुल्यकालिक डेटा सुलभ नहीं होगा
       *          पूल की प्रॉपर्टी परिवर्तित नहीं की जा सकती
       *          इस पूल के डेटासेट्स को रीड ओन्ली पे ही माउंट किया जा सकता है
       *          रीड ओन्ली पूल में लिखने के लिए, पूल के निर्यात और आयात
                  होना आवश्यक है |
       </translation>
    </message>
    <message>
        <source>Controls the system behavior in the event of catastrophic pool failure
This condition is typically a result of a loss of connectivity
to the underlying storage device(s) or a failure of all devices
within the pool. The behavior of such an event is determined as follows
wait
Blocks all I/O access until the device connectivity is recovered
and the errors are cleared. This is the default behavior
continue
Returns EIO to any new write I/O requests but allows reads to
any of the remaining healthy devices. Any write requests that
have yet to be committed to disk would be blocked.

panic
Prints out a message to the console and generates a system
crash dump.
</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>DialogfsCreate</name>
    <message>
        <source>New Filesystem</source>
        <translation>नया फाइल सिस्टम
            </translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>नाम:
            </translation>
    </message>
    <message>
        <source>Properties override at creation time:</source>
        <translation>प्रॉपर्टी निर्माण समय पर ओवरराइड:
            </translation>
    </message>
    <message>
        <source>Prevent auto mount (canmount=noauto)</source>
        <translation>ऑटो माउंट रोकें (केनमाउंट= नोऑटो)
            </translation>
    </message>
    <message>
        <source>Mountpoint</source>
        <translation>माउंट पॉइंट
            </translation>
    </message>
    <message>
        <source>none</source>
        <translation>कोई नहीं
            </translation>
    </message>
    <message>
        <source>legacy</source>
        <translation>उत्तरदान
            </translation>
    </message>
    <message>
        <source>[path]</source>
        <translation>[पथ]
            </translation>
    </message>
    <message>
        <source>Force UTF-8 only (utf8only=on)</source>
        <translation>यूटीएफ-8 पे ही दबाव डालें (यूटीएफ8ओन्ली=ऑन)
            </translation>
    </message>
    <message>
        <source>Unicode normalization =</source>
        <translation>यूनिकोड सामान्यीकरण =
            </translation>
    </message>
    <message>
        <source>formD</source>
        <translation>फॉर्मडी
            </translation>
    </message>
    <message>
        <source>formKCF</source>
        <translation>फॉर्मकेसीएफ
            </translation>
    </message>
    <message>
        <source>Copies =</source>
        <translation>कॉपीयाँ =
            </translation>
    </message>
    <message>
        <source>Deduplication (dedup=on)</source>
        <translation>प्रतिलिपिकरण हटाना (डीडूप=ऑन)
            </translation>
    </message>
    <message>
        <source>Compression =</source>
        <translation>संक्षिप्तीकरण =
            </translation>
    </message>
    <message>
        <source>off</source>
        <translation>ऑफ
            </translation>
    </message>
    <message>
        <source>on</source>
        <translation>ऑन
            </translation>
    </message>
    <message>
        <source>lzjb</source>
        <translation>एलज़ीजेबी
            </translation>
    </message>
    <message>
        <source>zle</source>
        <translation>ज़ीएलई
            </translation>
    </message>
    <message>
        <source>gzip</source>
        <translation>जीज़िप
            </translation>
    </message>
    <message>
        <source>gzip-1</source>
        <translation>जीज़िप-1
            </translation>
    </message>
    <message>
        <source>gzip-2</source>
        <translation>जीज़िप-2
            </translation>
    </message>
    <message>
        <source>gzip-3</source>
        <translation>जीज़िप-3
            </translation>
    </message>
    <message>
        <source>gzip-4</source>
        <translation>जीज़िप-4
            </translation>
    </message>
    <message>
        <source>gzip-5</source>
        <translation>जीज़िप-5
            </translation>
    </message>
    <message>
        <source>gzip-6</source>
        <translation>जीज़िप-6
            </translation>
    </message>
    <message>
        <source>gzip-7</source>
        <translation>जीज़िप-7
            </translation>
    </message>
    <message>
        <source>gzip-8</source>
        <translation>जीज़िप-8
            </translation>
    </message>
    <message>
        <source>gzip-9</source>
        <translation>जीज़िप-9
            </translation>
    </message>
</context>
<context>
    <name>ZManagerWindow</name>
    <message>
        <source>PCBSD Disk Manager</source>
        <translation>पी.सी-बी.एस.डी डिस्क प्रबंधक
            </translation>
    </message>
    <message>
        <source>Disks</source>
        <translation>डिस्क
            </translation>
    </message>
    <message>
        <source>Device</source>
        <translation>उपकर्ण
            </translation>
    </message>
    <message>
        <source>State</source>
        <translation>स्तिथि
            </translation>
    </message>
    <message>
        <source>ZFS Pools</source>
        <translation>ज़ीएफएस पूल
            </translation>
    </message>
    <message>
        <source>Pool Name</source>
        <translation>पूल का नाम
            </translation>
    </message>
    <message>
        <source>Status:</source>
        <translation>अवस्था:
            </translation>
    </message>
    <message>
        <source>TextLabel</source>
        <translation>मूलशब्द लेबेल
            </translation>
    </message>
    <message>
        <source>Clear Error</source>
        <translation>सॉफ त्रुटि
            </translation>
    </message>
    <message>
        <source>poolname</source>
        <translation>पूलनाम
            </translation>
    </message>
    <message>
        <source>ZFS Filesystems</source>
        <translation>ज़ीएफएस फाइलसिस्टम
            </translation>
    </message>
    <message>
        <source>Filesystems</source>
        <translation>फाइलसिस्टम
            </translation>
    </message>
    <message>
        <source>Status</source>
        <translation>अवस्था
            </translation>
    </message>
    <message>
        <source>Refreshing...</source>
        <translation>ताज़ा हो रहा है...
            </translation>
    </message>
    <message>
        <source>No pools available, right click to create a new one...</source>
        <translation>कोई पूल उपलब्ध नहीं है, नये का निर्माण करने के लिए राइट क्लिक करें...
            </translation>
    </message>
    <message>
        <source> (No media in drive)</source>
        <translation> (ड्राइव में कोई मीडीया नहीं है)
            </translation>
    </message>
    <message>
        <source>Avaliable</source>
        <translation>उपलब्ध
            </translation>
    </message>
    <message>
        <source>No disk</source>
        <translation>कोई डिस्क नहीं है
            </translation>
    </message>
    <message>
        <source>ZPool: </source>
        <translation>ज़ीपूल
            </translation>
    </message>
    <message>
        <source>Sliced</source>
        <translation>कटा हुआ
            </translation>
    </message>
    <message>
        <source>Mounted: </source>
        <translation>माउंट किया हुआ:
            </translation>
    </message>
    <message>
        <source>Available</source>
        <translation>उपलब्ध
            </translation>
    </message>
    <message>
        <source>Unmounted</source>
        <translation>अनमाउंट किया हुआ
            </translation>
    </message>
    <message>
        <source>Partitioned</source>
        <translation>विभाजित
            </translation>
    </message>
    <message>
        <source>(Exported)
</source>
        <translation>(निर्यात)
            </translation>
    </message>
    <message>
        <source>(Destroyed)
</source>
        <translation>(नष्ट)
            </translation>
    </message>
    <message>
        <source>Offline</source>
        <translation>ऑफलाइन
            </translation>
    </message>
    <message>
        <source>Online</source>
        <translation>ऑनलाइन
            </translation>
    </message>
    <message>
        <source>Degraded</source>
        <translation>पदावनत
            </translation>
    </message>
    <message>
        <source>Faulted</source>
        <translation>ग़लत
            </translation>
    </message>
    <message>
        <source>Removed</source>
        <translation>हटाया
            </translation>
    </message>
    <message>
        <source>Unavailable</source>
        <translation>अनुपलब्ध
            </translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation>अज्ञात
            </translation>
    </message>
    <message>
        <source>Create new pool</source>
        <translation>नए पूल बनाएँ
            </translation>
    </message>
    <message>
        <source>Rename pool</source>
        <translation>पूल का नाम बदलें
            </translation>
    </message>
    <message>
        <source>Destroy pool</source>
        <translation>पूल नष्ट करें
            </translation>
    </message>
    <message>
        <source>Add devices...</source>
        <translation>उपकर्ण जोड़ें...
            </translation>
    </message>
    <message>
        <source>Add log devices...</source>
        <translation>लॉग उपकर्ण जोड़ें...
            </translation>
    </message>
    <message>
        <source>Add cache devices...</source>
        <translation>कैश उपकर्ण जोड़ें...
            </translation>
    </message>
    <message>
        <source>Add spare devices...</source>
        <translation>अतिरिक्त उपकर्ण जोड़ें...
            </translation>
    </message>
    <message>
        <source>Scrub</source>
        <translation>स्क्रब
            </translation>
    </message>
    <message>
        <source>Export pool</source>
        <translation>पूल निर्यात करें
            </translation>
    </message>
    <message>
        <source>Import pool</source>
        <translation>पूल आयात करें
            </translation>
    </message>
    <message>
        <source>Recover destroyed pool</source>
        <translation>नष्ट पूल को पुन:प्राप्त करें
            </translation>
    </message>
    <message>
        <source>Properties...</source>
        <translation>प्रॉपर्टीस...
            </translation>
    </message>
    <message>
        <source>Attach (mirror) device...</source>
        <translation>(दर्पण) उपकर्ण जोड़ें...
            </translation>
    </message>
    <message>
        <source>Detach from mirror</source>
        <translation>दर्पण से अलग करें
            </translation>
    </message>
    <message>
        <source>Take offline</source>
        <translation>ऑफ़लाइन ले
            </translation>
    </message>
    <message>
        <source>Bring online</source>
        <translation>ऑनलाइन लाओ
            </translation>
    </message>
    <message>
        <source>Remove</source>
        <translation>हटायें
            </translation>
    </message>
    <message>
        <source>zpool Menu</source>
        <translation>ज़ीपूल मेनू
            </translation>
    </message>
    <message>
        <source>Device Menu</source>
        <translation>उपकर्ण मेनू
            </translation>
    </message>
    <message>
        <source>Unmount</source>
        <translation>अनमाउंट
            </translation>
    </message>
    <message>
        <source>Mount</source>
        <translation>माउंट
            </translation>
    </message>
    <message>
        <source>Create MBR partition table</source>
        <translation>एमबीआर विभाजन तालिका बनाएँ
            </translation>
    </message>
    <message>
        <source>Create GPT partition table</source>
        <translation>जीपीटी विभाजन तालिका बनाएँ
            </translation>
    </message>
    <message>
        <source>Create BSD partition table</source>
        <translation>बीएसडी विभाजन तालिका बनाएँ
            </translation>
    </message>
    <message>
        <source>Delete Partition Table</source>
        <translation>विभाजन तालिका हटायें
            </translation>
    </message>
    <message>
        <source>Add new slice</source>
        <translation>नया टुकड़ा जोड़ें
            </translation>
    </message>
    <message>
        <source>Add new partition</source>
        <translation>नया विभाजन जोड़ें
            </translation>
    </message>
    <message>
        <source>Destroy this slice</source>
        <translation>यह टुकड़ा नष्ट करें
            </translation>
    </message>
    <message>
        <source>Destroy this partition</source>
        <translation>यह विभाजन नष्ट करें
            </translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>चेतावनी
            </translation>
    </message>
    <message>
        <source>This operation cannot be undone.
OK to destroy the slice/partition?</source>
        <translation>इस कार्रवाई को पूर्ववत नहीं किया जा सकता
टुकड़ा / विभाजन को नष्ट करने के लिए ठीक है?
            </translation>
    </message>
    <message>
        <source>Error report</source>
        <translation>त्रुटि रिपोर्ट
            </translation>
    </message>
    <message>
        <source>Create new zpool</source>
        <translation>नया ज़ीपूल बनाएँ
            </translation>
    </message>
    <message>
        <source>Important information</source>
        <translation>महत्वपूर्ण जानकारी
            </translation>
    </message>
    <message>
        <source>The pool was imported in read-only mode, therefore attempting to destroy the pool will leave the pool in the state it was when imported, not necessarily destroyed.</source>
        <translation>पूल केवल रीड ओन्ली मोड में आयात किया गया था, इसलिए पूल को नष्ट करने के प्रयास से , पूल उस स्तिथि में आजाएगा जब वो आयात होते वक़्त था, नष्ट ज़रूरी नहीं
            </translation>
    </message>
    <message>
        <source>Attach mirror devices to </source>
        <translation>दर्पण उपकरणों को संलग्न करें
            </translation>
    </message>
    <message>
        <source>The pool was imported in read-only mode, therefore attempting to export the pool will leave the pool in the state it was when imported, not necessarily exported.</source>
        <translation>पूल केवल रीड ओन्ली मोड में आयात किया गया था, इसलिए पूल को नष्ट करने के प्रयास से , पूल उस स्तिथि में आजाएगा जब वो निर्यात होते वक़्त था, नष्ट ज़रूरी नहीं
            </translation>
    </message>
    <message>
        <source>Recover destroyed pool as...</source>
        <translation>नष्ट पूल को ऐसे पुन:प्राप्त करें...
            </translation>
    </message>
    <message>
        <source>Import pool as...</source>
        <translation>पूल को ऐसे आयात करें...
            </translation>
    </message>
    <message>
        <source>The pool was imported in read-only mode, it cannot be renamed.</source>
        <translation>पूल रीड ओन्ली मोड में आयात किया गया था, इसका नाम बदला नहीं जा सकता
            </translation>
    </message>
    <message>
        <source>Add more devices to zpool</source>
        <translation>ज़ीपूल में और उपकर्ण जोड़ें
            </translation>
    </message>
    <message>
        <source>Add log devices to zpool</source>
        <translation>ज़ीपूल में लॉग उपकर्ण जोड़ें
            </translation>
    </message>
    <message>
        <source>Add cache devices to zpool</source>
        <translation>ज़ीपूल में कैश उपकर्ण जोड़ें
            </translation>
    </message>
    <message>
        <source>Add spare devices to zpool</source>
        <translation>ज़ीपूल में अतिरिक्त उपकर्ण जोड़ें
            </translation>
    </message>
    <message>
        <source>[Mounted]</source>
        <translation>[माउंट कर दिया गया]
            </translation>
    </message>
    <message>
        <source>[Not Mounted]</source>
        <translation>[माउंट नहीं किया गया]
            </translation>
    </message>
    <message>
        <source> of </source>
        <translation> का 
            </translation>
    </message>
    <message>
        <source>Rename dataset</source>
        <translation>डाटासेट का नाम बदलें
            </translation>
    </message>
    <message>
        <source>Create new dataset</source>
        <translation>नया डाटासेट बनायें
            </translation>
    </message>
    <message>
        <source>Create a clone dataset</source>
        <translation>क्लोन डाटासेट बनायें
            </translation>
    </message>
    <message>
        <source>Destroy dataset</source>
        <translation>डाटासेट नष्ट करें
            </translation>
    </message>
    <message>
        <source>Promote filesystem</source>
        <translation>फाइल सिस्टम को बढ़ावा दे
            </translation>
    </message>
    <message>
        <source>Take a snapshot</source>
        <translation>एक स्नैपशॉट ले
            </translation>
    </message>
    <message>
        <source>Destroy snapshot</source>
        <translation>स्नैपशॉट नष्ट करें
            </translation>
    </message>
    <message>
        <source>Rollback to this snapshot</source>
        <translation>इस स्नैपशॉट में रॉलबैक करें
            </translation>
    </message>
    <message>
        <source>Edit properties</source>
        <translation>प्रॉपर्टीस संपादित करें
            </translation>
    </message>
    <message>
        <source>zfs Menu</source>
        <translation>ज़ीएफएस मेनू
            </translation>
    </message>
    <message>
        <source>Confirmation</source>
        <translation>पुष्टि
            </translation>
    </message>
    <message>
        <source>Take a new snapshot</source>
        <translation>एक नया स्नैपशॉट ले
            </translation>
    </message>
    <message>
        <source>New name</source>
        <translation>नया नाम
            </translation>
    </message>
    <message>
        <source>This operation cannot be undone and will cause all data added after the snapshot to be lost.
Any snapshots created after this one will be deleted, along with any clone filesystems that depend on them.
</source>
        <translation>इस कार्रवाई को पूर्ववत नहीं किया जा सकता और इसके कारण स्नैपशॉट के बाद का सभी डेटा खो जाएगा |
इस स्नैपशॉट के बाद कोई भी बनाया स्नैपशॉट हटा दिया जाएगा, उन पर निर्भर किसी भी क्लोन फाइल सिस्टम के साथ |
            </translation>
    </message>
    <message>
        <source>This pool had been destroyed, and its disks may have been reused. Attempting to recover will destroy any new data that could&apos;ve been stored in the devices that were reused and cannot be recovered.
Proceed with recovery?</source>
        <translation>इस पूल को नष्ट कर दिया गया था, और इसकी डिस्क को शाएद दोबारा इस्तेमाल किया गया है | पुन:प्राप्त करने की कोशिश करने से, जो भी डाटा उपकरणों में जो दोबारा इस्तेमाल किए जा रहे थे, संग्रहीत नहीं हो पाया था, वो सब नया डाटा नष्ट हो जाएगा, और उसको पुन:प्राप्त नहीं किया जा सकता |
पुन:प्राप्ति के साथ आगे बढ़ें?
            </translation>
    </message>
    <message>
        <source>An error was detected while executing &apos;%1&apos;:

</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>An error was detected while executing &apos;zpool&apos;:

</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>An error was detected while executing &apos;zfs&apos;:

</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>This operation cannot be undone and will cause data loss.

You are about to perform the following operation(s):

</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>

Are you sure you want to proceed?

</source>
        <translation type="unfinished"></translation>
    </message>
</context>
</TS>
