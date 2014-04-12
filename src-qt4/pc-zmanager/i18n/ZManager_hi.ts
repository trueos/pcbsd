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
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Read-only property that identifies the amount of disk space that is used by
children of this dataset, which would be freed if all the dataset&apos;s children
were destroyed.</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Read-only property that identifies the amount of disk space that is used by
a dataset itself, which would be freed if the dataset was destroyed, after
first destroying any snapshots and removing any refreservation reservations.</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Read-only property that identifies the amount of disk space that is used by a
refreservation set on a dataset, which would be freed if the refreservation was
removed.</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Read-only property that identifies the amount of disk space that is consumed
by snapshots of a dataset. In particular, it is the amount of disk space that
would be freed if all of this dataset&apos;s snapshots were destroyed. Note that
this value is not simply the sum of the snapshots&apos; used properties, because
space can be shared by multiple snapshots.</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Identifies the on-disk version of a file system, which is independent of the
pool version. This property can only be set to a later version that is available
from the supported software release.</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>For volumes, specifies the logical size of the volume.</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>For volumes, specifies the block size of the volume.</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Indicates whether a dataset has been added to a jail.</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>This property is not supported on FreeBSD.</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>The file system is marked to be destroyed after unmount.</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>The compression ratio achieved for the referenced space of this
dataset, expressed as a multiplier.</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>The amount of referenced space written to this dataset since the
previous snapshot.</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>This property is set to the number of user holds on this snapshot.
</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Provide a hint to ZFS about handling of synchronous requests in this
dataset.  If logbias is set to latency (the default), ZFS will use
pool log devices (if configured) to handle the requests at low
latency. If logbias is set to throughput, ZFS will not use configured
pool log devices.  ZFS will instead optimize synchronous operations
for global pool throughput and efficient use of resources.</source>
        <translation type="unfinished">
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
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>This property cannot be changed after the system is created.</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>User Property</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Unknown property or information not available.
Read ZFS manual for more information.</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>[Invalid]</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>[Inherit]</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>[Added]</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>[Changed]</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>User property</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Device Menu</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Add new user property</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Delete user property</source>
        <translation type="unfinished">
            </translation>
    </message>
</context>
<context>
    <name>DialogMount</name>
    <message>
        <source>Dialog</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Device to mount:</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>(Device)</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Mount location:</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>...</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Select Directory</source>
        <translation type="unfinished">
            </translation>
    </message>
</context>
<context>
    <name>DialogName</name>
    <message>
        <source>Dialog</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Pool name:</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Do not mount any filesystems</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Read only pool</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Import options:</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Mount with alternative root:</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Mount all filesystems automatically</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Mounting behavior:</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Force operation if error (use with caution)</source>
        <translation type="unfinished">
            </translation>
    </message>
</context>
<context>
    <name>DialogNewPool</name>
    <message>
        <source>Dialog</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Selected</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Pool name:</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Configuration type:</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Striped</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Mirror</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Raid-Z (single parity)</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Raid-Z2 (double parity)</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Raid-Z3 (triple parity)</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Cache</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Log</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Spare</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Error Description Here</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Must provide a valid pool name</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Must select exactly %1 devices</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Must select at least one device</source>
        <translation type="unfinished">
            </translation>
    </message>
</context>
<context>
    <name>DialogPartition</name>
    <message>
        <source>Add Partition</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Add new slice/partition</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>100 GB</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Device:</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>ada0</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Largest available free block:</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Total Size:</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Current layout:</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Designation</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Start Sector</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Sector Count</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Size</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Color</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>New partition Size:</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>bytes</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>kB</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>MB</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>GB</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>TB</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Partition type:</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>freebsd-zfs</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>freebsd-ufs</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>freebsd</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>freebsd-boot</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>freebsd-swap</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>bios-boot</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>efi</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Create and initialize a new filesystem</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>** FREE **</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>No free space!</source>
        <translation type="unfinished">
            </translation>
    </message>
</context>
<context>
    <name>DialogProp</name>
    <message>
        <source>Edit Pool Properties</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Property</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Value</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Total size of the storage pool.</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Percentage of pool space used.</source>
        <translation type="unfinished">
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
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>The current health of the pool.</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>A unique identifier for the pool.</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>The current on-disk version of the pool. This can be increased, but
never decreased. The preferred method of updating pools is with the
&quot;zpool upgrade&quot; command, though this property can be used when a
specific version is needed for backwards compatibility. This property
can be any number between 1 and the current version reported by
&quot;zpool upgrade -v&quot;.</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Identifies the default bootable dataset for the root pool. This
property is expected to be set mainly by the installation and upgrade
programs.</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Controls whether a non-privileged user is granted access based on the
dataset permissions defined on the dataset. See zfs(8) for more
information on ZFS delegated administration.</source>
        <translation type="unfinished">
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
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Controls whether information about snapshots associated with this
pool is output when &quot;zfs list&quot; is run without the -t option. The
default value is off.</source>
        <translation type="unfinished">
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
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Threshold for the number of block ditto copies. If the reference
count for a deduplicated block increases above this number, a new
ditto copy of this block is automatically stored. Default setting is
0.</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>The deduplication ratio specified for a pool, expressed as a multiplier. For
example, a value of 1.76 indicates that 1.76 units of data were stored but
only 1 unit of disk space was actually consumed. See zfs(8) for a description
of the deduplication feature.</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Number of blocks within the pool that are not allocated.</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Amount of storage space within the pool that has been physically allocated.</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>An arbitrary comment string set by the administrator.</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>This property has currently no value on FreeBSD.</source>
        <translation type="unfinished">
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
        <translation type="unfinished">
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
        <translation type="unfinished">
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
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Name:</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Properties override at creation time:</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Prevent auto mount (canmount=noauto)</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Mountpoint</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>none</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>legacy</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>[path]</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Force UTF-8 only (utf8only=on)</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Unicode normalization =</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>formD</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>formKCF</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Copies =</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Deduplication (dedup=on)</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Compression =</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>off</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>on</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>lzjb</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>zle</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>gzip</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>gzip-1</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>gzip-2</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>gzip-3</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>gzip-4</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>gzip-5</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>gzip-6</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>gzip-7</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>gzip-8</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>gzip-9</source>
        <translation type="unfinished">
            </translation>
    </message>
</context>
<context>
    <name>ZManagerWindow</name>
    <message>
        <source>PCBSD Disk Manager</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Disks</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Device</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>State</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>ZFS Pools</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Pool Name</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Status:</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>TextLabel</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Clear Error</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>poolname</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>ZFS Filesystems</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Filesystems</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Status</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Refreshing...</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>No pools available, right click to create a new one...</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source> (No media in drive)</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Avaliable</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>No disk</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>ZPool: </source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Sliced</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Mounted: </source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Available</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Unmounted</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Partitioned</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>(Exported)
</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>(Destroyed)
</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Offline</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Online</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Degraded</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Faulted</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Removed</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Unavailable</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Create new pool</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Rename pool</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Destroy pool</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Add devices...</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Add log devices...</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Add cache devices...</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Add spare devices...</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Scrub</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Export pool</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Import pool</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Recover destroyed pool</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Properties...</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Attach (mirror) device...</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Detach from mirror</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Take offline</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Bring online</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Remove</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>zpool Menu</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Device Menu</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Unmount</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Mount</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Create MBR partition table</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Create GPT partition table</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Create BSD partition table</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Delete Partition Table</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Add new slice</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Add new partition</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Destroy this slice</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Destroy this partition</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Warning</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>This operation cannot be undone.
OK to destroy the slice/partition?</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Error report</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Create new zpool</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Important information</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>The pool was imported in read-only mode, therefore attempting to destroy the pool will leave the pool in the state it was when imported, not necessarily destroyed.</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Attach mirror devices to </source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>The pool was imported in read-only mode, therefore attempting to export the pool will leave the pool in the state it was when imported, not necessarily exported.</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Recover destroyed pool as...</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Import pool as...</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>The pool was imported in read-only mode, it cannot be renamed.</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Add more devices to zpool</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Add log devices to zpool</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Add cache devices to zpool</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Add spare devices to zpool</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>[Mounted]</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>[Not Mounted]</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source> of </source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Rename dataset</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Create new dataset</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Create a clone dataset</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Destroy dataset</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Promote filesystem</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Take a snapshot</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Destroy snapshot</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Rollback to this snapshot</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Edit properties</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>zfs Menu</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Confirmation</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>Take a new snapshot</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>New name</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>This operation cannot be undone and will cause all data added after the snapshot to be lost.
Any snapshots created after this one will be deleted, along with any clone filesystems that depend on them.
</source>
        <translation type="unfinished">
            </translation>
    </message>
    <message>
        <source>This pool had been destroyed, and its disks may have been reused. Attempting to recover will destroy any new data that could&apos;ve been stored in the devices that were reused and cannot be recovered.
Proceed with recovery?</source>
        <translation type="unfinished">
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
