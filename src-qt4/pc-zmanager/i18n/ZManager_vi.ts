<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.0">
<context>
    <name>DialogFSProp</name>
    <message>
        <source>Dialog</source>
        <translation>Thoại</translation>
    </message>
    <message>
        <source>Apply changes recursively to all descendants</source>
        <translation>Áp dụng thay đổi đệ quy cho tất cả con cháu</translation>
    </message>
    <message>
        <source>Property</source>
        <translation>Tài sản</translation>
    </message>
    <message>
        <source>Value</source>
        <translation>Giá trị</translation>
    </message>
    <message>
        <source>Source</source>
        <translation>Nguồn</translation>
    </message>
    <message>
        <source>Add user property</source>
        <translation>Thêm bất động sản sử dụng</translation>
    </message>
    <message>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <source>Remove user property</source>
        <translation>Loại bỏ tài sản sử dụng</translation>
    </message>
    <message>
        <source>Inherit from parent</source>
        <translation>Kế thừa từ cha mẹ</translation>
    </message>
    <message>
        <source>Revert changes</source>
        <translation>trở lại thay đổi</translation>
    </message>
    <message>
        <source>Controls how ACL entries are inherited when files and directories are created. The values are
discard, noallow, restricted, and passthrough. For a description of these values, see ACL Properties.</source>
        <translation>Kiểm soát cách mục ACL được thừa hưởng khi các tập tin và thư mục được tạo ra. Các giá trị
loại bỏ, noallow, hạn chế, và passthrough. Cho một mô tả của các giá trị, xem ACL Properties.</translation>
    </message>
    <message>
        <source>Controls how an ACL entry is modified during a chmod operation.
The values are discard, groupmask, and passthrough. For a description of
these values, see ACL Properties.</source>
        <translation>Kiểm soát như thế nào một ACL được sửa đổi trong một hoạt động chmod.
Các giá trị là loại bỏ, groupmask, và passthrough. Cho một mô tả
các giá trị, xem ACL Properties.</translation>
    </message>
    <message>
        <source>Controls whether the access time for files is updated when they are read.
Turning this property off avoids producing write traffic when reading files and
can result in significant performance gains, though it might confuse mailers and
similar utilities.</source>
        <translation>Điều khiển thời gian truy cập cho các tập tin được cập nhật khi chúng được đọc.
Chuyển tài sản này ra tránh sản xuất ghi giao thông khi đọc tập tin và
có thể dẫn đến hiệu suất vượt trội, mặc dù nó có thể gây nhầm lẫn bưu phẩm và
tiện ích giống nhau.</translation>
    </message>
    <message>
        <source>Read-only property that identifies the amount of disk space available to a dataset
and all its children, assuming no other activity in the pool. Because disk space is
shared within a pool, available space can be limited by various factors including
physical pool size, quotas, reservations, and other datasets within the pool.</source>
        <translation>Thuộc tính chỉ đọc xác định số lượng không gian đĩa có sẵn cho một bộ dữ liệu
và tất cả các con của nó, giả sử không có hoạt động nào khác trong hồ bơi. Bởi vì không gian đĩa
chia sẻ trong một hồ bơi, không gian có sẵn có thể được giới hạn bởi các yếu tố khác nhau bao gồm
kích thước vật hồ bơi, hạn ngạch, đặt phòng, và các bộ dữ liệu khác trong hồ bơi.</translation>
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
        <translation>Nếu tài sản này được thiết lập để tắt, hệ thống tập tin không thể được gắn kết,
và bỏ qua &apos;zfs mount-a&apos;. Thiết lập thuộc tính này để tắt là
tương tự như thiết lập thuộc tính láp không ai sánh kịp, ngoại trừ các
bộ dữ liệu vẫn còn có một tài sản láp bình thường, có thể được thừa hưởng
Thiết lập thuộc tính này để tắt cho phép bộ dữ liệu được sử dụng chỉ như một
cơ chế thừa kế tài sản. Một ví dụ về thiết lập canmount = off là
có hai bộ dữ liệu với láp cùng, để các con của
cả hai bộ dữ liệu xuất hiện trong cùng một thư mục, nhưng có thể có khác nhau
kế thừa các đặc tính.</translation>
    </message>
    <message>
        <source>Controls the checksum used to verify data integrity. The default
value is on, which automatically selects an appropriate algorithm
(currently, fletcher4, but this may change in future releases). The
value off disables integrity checking on user data. Disabling
checksums is NOT a recommended practice.</source>
        <translation>Kiểm soát kiểm tra được sử dụng để xác minh tính toàn vẹn dữ liệu.
 mặc định giá trị trên, tự động lựa chọn một thuật toán thích hợp
(hiện tại, fletcher4, nhưng điều này có thể thay đổi trong phiên bản tương lai).
giá trị ra vô hiệu hóa kiểm tra trên dữ liệu người dùng toàn vẹn. Vô hiệu hóa
tổng kiểm tra là không một thực hành được đề nghị.</translation>
    </message>
    <message>
        <source>Enables or disables compression for a dataset. The values are on, off, lzjb, zle, gzip,
and gzip-N. Currently, setting this property to lzjb, gzip, or gzip-N has the same
effect as setting this property to on. Enabling compression on a file system with
existing data only compresses new data. Existing data remains uncompressed.</source>
        <translation>Cho phép hoặc vô hiệu hóa nén cho một tập dữ liệu. Các giá trị là bật, tắt, lzjb, zle, gzip,
và gzip-N. Hiện nay, thiết lập thuộc tính này để lzjb, gzip, hoặc gzip-N có cùng
ảnh hưởng như thiết lập thuộc tính này trên. Cho phép nén trên một hệ thống tập tin với
dữ liệu hiện có chỉ nén dữ liệu mới. Dữ liệu hiện có vẫn không nén.</translation>
    </message>
    <message>
        <source>Read-only property that identifies the compression ratio achieved for a dataset,
expressed as a multiplier. Compression can be enabled by the zfs set compression=on
dataset command. The value is calculated from the logical size of all files and the
amount of referenced physical data. It includes explicit savings through the use of
the compression property.</source>
        <translation>Thuộc tính chỉ đọc xác định tỉ lệ nén đạt được cho một bộ dữ liệu,
thể hiện như một số nhân. Nén có thể được kích hoạt bằng cách nén zfs set=
lệnh bộ dữ liệu. Giá trị được tính từ kích thước hợp lý của tất cả các tập tin và thư mục
số lượng dữ liệu vật lý tham chiếu. Nó bao gồm tiết kiệm rõ ràng thông qua việc sử dụng
tài sản nén.</translation>
    </message>
    <message>
        <source>Sets the number of copies of user data per file system. These copies
are in addition to any pool-level redundancy. Disk space used by
multiple copies of user data is charged to the corresponding file and
dataset, and counts against quotas and reservations. In addition, the
used property is updated when multiple copies are enabled. Consider setting
this property when the file system is created because changing this property
on an existing file system only affects newly written data.</source>
        <translation>Đặt số lượng bản sao của dữ liệu người dùng trên hệ thống tập tin. Các bản sao
được thêm vào bất kỳ dự phòng hồ bơi cấp. Không gian đĩa được sử dụng bởi
nhiều bản sao của dữ liệu người dùng được tính vào các tập tin và tương ứng
bộ dữ liệu, và số lượng so với hạn ngạch và đặt phòng. Ngoài ra,
tài sản được sử dụng được cập nhật khi nhiều bản sao được kích hoạt. Xem xét việc thiết
khách sạn này khi hệ thống tập tin được tạo ra bởi vì thay đổi này sở hữu
trên một hệ thống tập tin hiện có chỉ ảnh hưởng đến dữ liệu mới bằng văn bản.</translation>
    </message>
    <message>
        <source>Read-only property that identifies the date and time that a dataset was created.</source>
        <translation>Thuộc tính chỉ đọc để nhận dạng ngày tháng và thời gian mà một bộ dữ liệu được tạo ra.</translation>
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
        <translation>Cấu hình pc cho một tập dữ liệu. Giá trị mặc ​​định là tắt.
Checksum pc mặc định là sha256 (điều này có thể thay đổi trong
trong tương lai). Khi dedup được kích hoạt, kiểm tra xác định đây sẽ ghi đè
tài sản kiểm tra. Thiết lập giá trị để xác minh có cùng
hiệu lực các sha256 thiết lập, xác minh.
Nếu thiết lập để xác minh, ZFS sẽ làm một comparsion byte-to-byte trong trường hợp
hai khối có cùng một chữ ký để đảm bảo các nội dung khối
giống hệt nhau.</translation>
    </message>
    <message>
        <source>The devices property is currently not supported on FreeBSD.</source>
        <translation>Tài sản thiết bị hiện đang không được hỗ trợ trên FreeBSD.</translation>
    </message>
    <message>
        <source>Controls whether processes can be executed from within this file
system. The default value is on.</source>
        <translation>Điều khiển quá trình có thể được thực hiện từ bên trong tập tin này
hệ thống. Giá trị mặc định là trên.</translation>
    </message>
    <message>
        <source>Read-only property that indicates whether a file system, clone, or snapshot
is currently mounted.</source>
        <translation>Thuộc tính chỉ đọc cho biết có một hệ thống tập tin, nhân bản, hoặc ảnh chụp
hiện đang gắn kết.</translation>
    </message>
    <message>
        <source>Controls the mount point used for this file system. When the mountpoint property
is changed for a file system, the file system and any descendents that inherit the
mount point are unmounted. If the new value is legacy, then they remain unmounted.
Otherwise, they are automatically remounted in the new location if the property was
previously legacy or none, or if they were mounted before the property was changed.
In addition, any shared file systems are unshared and shared in the new location.</source>
        <translation>Kiểm soát các điểm gắn kết sử dụng cho hệ thống tập tin này. Khi bất động sản láp
được thay đổi cho một hệ thống tập tin, hệ thống tập tin và các con cháu kế thừa
mount point là chưa lắp ráp. Nếu giá trị mới là di sản, sau đó họ vẫn chưa lắp ráp.
Nếu không, họ sẽ được tự động remounted trong vị trí mới nếu tài sản được
trước di sản hoặc không có, hoặc nếu họ đã được gắn vào trước khi tài sản đã được thay đổi
.Ngoài ra, bất kỳ hệ thống tập tin được chia sẻ là không chia sẻ và được chia sẻ trong vị trí mới.</translation>
    </message>
    <message>
        <source>Controls what is cached in the primary cache (ARC). Possible values are all,
none, and metadata. If set to all, both user data and metadata are cached. If
set to none, neither user data nor metadata is cached. If set to metadata, only
metadata is cached.</source>
        <translation>Kiểm soát những gì được lưu trữ trong bộ nhớ cache chính (ARC). Giá trị có thể là tất cả,
không có, và siêu dữ liệu. Nếu thiết lập cho tất cả, cả dữ liệu người dùng và siêu dữ liệu được lưu trữ. Nếu
thiết lập để không, không dữ liệu người dùng cũng không siêu dữ liệu được lưu trữ. Nếu thiết lập siêu dữ liệu, chỉ
siêu dữ liệu được lưu trữ.</translation>
    </message>
    <message>
        <source>Read-only property for cloned file systems or volumes that identifies
the snapshot from which the clone was created. The origin cannot be
destroyed (even with the -r or -f option) as long as a clone exists.
Non-cloned file systems have an origin of none.</source>
        <translation>Thuộc tính chỉ đọc các file hệ thống nhân bản vô tính hoặc khối lượng xác định
ảnh chụp từ đó các bản sao đã được tạo ra. Nguồn gốc không thể
phá hủy (ngay cả với các-r hoặc tùy chọn-f) miễn là một bản sao tồn tại.
Hệ thống tập tin không nhân bản vô tính có một nguồn gốc của không.</translation>
    </message>
    <message>
        <source>Limits the amount of disk space a dataset and its descendents can consume.
This property enforces a hard limit on the amount of disk space used,
including all space consumed by descendents, such as file systems and snapshots.
Setting a quota on a descendent of a dataset that already has a quota does not
override the ancestor&apos;s quota, but rather imposes an additional limit.</source>
        <translation>Giới hạn số lượng không gian đĩa một bộ dữ liệu và các hậu duệ của nó có thể tiêu thụ.
Khách sạn này thi hành một giới hạn cứng về số lượng không gian đĩa được sử dụng,
bao gồm tất cả các không gian tiêu thụ bởi con cháu, chẳng hạn như hệ thống tập tin và các bức ảnh chụp.
Thiết lập một hạn ngạch về một hậu duệ của một bộ dữ liệu mà đã có một hạn ngạch không
ghi đè lên hạn ngạch của tổ tiên, nhưng thay vì áp đặt một giới hạn bổ sung.</translation>
    </message>
    <message>
        <source>Controls whether a dataset can be modified. When set to on,
no modifications can be made.</source>
        <translation>Kiểm soát xem một bộ dữ liệu có thể được sửa đổi. Khi thiết lập trên,
không có sửa đổi có thể được thực hiện.</translation>
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
        <translation>Quy định cụ thể một kích thước khối đề nghị cho các tập tin trong hệ thống tập tin.
tài sản được thiết kế chỉ để sử dụng với khối lượng công việc cơ sở dữ liệu
tập tin truy cập trong hồ sơ kích thước cố định. ZFS tự động điều chỉnh khối
kích cỡ theo thuật toán nội bộ tối ưu hóa cho truy cập điển hình
mô hình.
Sử dụng tài sản này cho các hệ thống tập tin mục đích chung là
 mạnh mẽkhuyến khích, và có thể ảnh hưởng đến hình
.Kích thước quy định phải có một sức mạnh của hai lớn hơn hoặc bằng
512 và nhỏ hơn hoặc bằng 128 Kb.</translation>
    </message>
    <message>
        <source>Read-only property that identifies the amount of data accessible by a
dataset, which might or might not be shared with other datasets in the
pool.</source>
        <translation>Thuộc tính chỉ đọc xác định số lượng dữ liệu truy cập bằng một
bộ dữ liệu, mà có thể hoặc không có thể được chia sẻ với các bộ dữ liệu khác trong
hồ bơi.</translation>
    </message>
    <message>
        <source>Sets the amount of disk space that a dataset can consume. This property
enforces a hard limit on the amount of space used. This hard limit does
not include disk space used by descendents, such as snapshots and clones.</source>
        <translation>Thiết lập dung lượng ổ đĩa mà một bộ dữ liệu có thể tiêu thụ. Này sở hữu
thực thi một giới hạn cứng về số lượng không gian sử dụng. Giới hạn cứng này không
không bao gồm không gian đĩa được sử dụng bởi con cháu, chẳng hạn như ảnh chụp nhanh và bắt chước.</translation>
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
        <translation>Đặt số tiền tối thiểu của không gian đĩa được đảm bảo để một bộ dữ liệu, không
bao gồm cả con cháu, chẳng hạn như ảnh chụp nhanh và bắt chước. Khi số lượng đĩa
không gian sử dụng là thấp hơn giá trị này, các dữ liệu được xử lý như thể nó đã được chiếm
số lượng không gian theo quy định của refreservation. Các phòng refreservation
được hạch toán trong không gian dữ liệu của cha mẹ đĩa được sử dụng, và tính chống lại
hạn ngạch các bộ dữ liệu của cha mẹ và đặt phòng.
Nếu refreservation được thiết lập, một bản chụp chỉ được phép nếu đủ hồ bơi miễn phí không gian
có sẵn bên ngoài phòng này để phù hợp với số lượng hiện tại của
byte tham chiếu trong tập dữ liệu.</translation>
    </message>
    <message>
        <source>Sets the minimum amount of disk space guaranteed to a dataset and its descendents.
When the amount of disk space used is below this value, the dataset is treated as if
it were using the amount of space specified by its reservation. Reservations are
accounted for in the parent dataset&apos;s disk space used, and count against the parent
dataset&apos;s quotas and reservations.</source>
        <translation>Đặt số tiền tối thiểu của không gian đĩa được đảm bảo để một bộ dữ liệu và con cháu của nó.
Khi số lượng không gian đĩa được sử dụng là thấp hơn giá trị này, các dữ liệu được xử lý như
nó đã được sử dụng số lượng không gian được xác định bởi phòng của mình. Đặt
chiếm trong không gian đĩa dữ liệu của cha mẹ sử dụng, và tính đối với phụ huynh
hạn ngạch bộ dữ liệu và đặt phòng.</translation>
    </message>
    <message>
        <source>Controls what is cached in the secondary cache (L2ARC). Possible values are all, none,
and metadata. If set to all, both user data and metadata are cached. If set to none,
neither user data nor metadata is cached. If set to metadata, only metadata is cached.</source>
        <translation>Kiểm soát những gì được lưu trữ trong bộ nhớ cache thứ cấp (L2ARC). Giá trị có thể là tất cả, không có,
và siêu dữ liệu. Nếu thiết lập cho tất cả, cả dữ liệu người dùng và siêu dữ liệu được lưu trữ. Nếu thiết lập để không,
không dữ liệu người dùng cũng không siêu dữ liệu được lưu trữ. Nếu thiết lập siêu dữ liệu, chỉ có siêu dữ liệu được lưu trữ.</translation>
    </message>
    <message>
        <source>Controls whether the setuid bit is honored in a file system.</source>
        <translation>Kiểm soát cho dù các bit setuid được vinh danh trong một hệ thống tập tin.</translation>
    </message>
    <message>
        <source>Controls whether a file system is available over NFS and what
options are used. If set to on, the zfs share command is invoked
with no options. Otherwise, the zfs share command is invoked with
options equivalent to the contents of this property.</source>
        <translation>Kiểm soát cho dù một hệ thống tập tin có sẵn trên NFS và những gì
tùy chọn được sử dụng. Nếu thiết lập trên, lệnh zfs phần được gọi
không có tùy chọn. Nếu không, lệnh zfs cổ phiếu được gọi với
tùy chọn tương ứng với nội dung của khách sạn này.</translation>
    </message>
    <message>
        <source>The sharesmb property has currently no effect on FreeBSD.</source>
        <translation>Tài sản sharesmb hiện không có tác dụng trên FreeBSD.</translation>
    </message>
    <message>
        <source>Controls whether the .zfs directory is hidden or visible in the root of the file system.</source>
        <translation>Điều khiển các thư mục zfs. Là ẩn hay nhìn thấy trong thư mục gốc của hệ thống tập tin.</translation>
    </message>
    <message>
        <source>Read-only property that identifies the dataset type as filesystem (file system or clone), volume, or snapshot.</source>
        <translation>Thuộc tính chỉ đọc để nhận dạng các loại dữ liệu như hệ thống tập tin (file hệ thống hoặc bản sao), khối lượng, hoặc ảnh chụp.</translation>
    </message>
    <message>
        <source>Read-only property that identifies the amount of disk space consumed by a dataset and all its descendents.</source>
        <translation>Thuộc tính chỉ đọc xác định số lượng không gian đĩa được tiêu thụ bởi một bộ dữ liệu và tất cả con cháu của nó.</translation>
    </message>
    <message>
        <source>Read-only property that identifies the amount of disk space that is used by
children of this dataset, which would be freed if all the dataset&apos;s children
were destroyed.</source>
        <translation>Thuộc tính chỉ đọc xác định số lượng không gian đĩa được sử dụng bởi
trẻ em của tập dữ liệu này, mà sẽ được trả tự do nếu tất cả trẻ em của bộ dữ liệu
đã bị phá hủy.</translation>
    </message>
    <message>
        <source>Read-only property that identifies the amount of disk space that is used by
a dataset itself, which would be freed if the dataset was destroyed, after
first destroying any snapshots and removing any refreservation reservations.</source>
        <translation>Thuộc tính chỉ đọc xác định số lượng không gian đĩa được sử dụng bởi
một bộ dữ liệu riêng của mình, mà sẽ được giải phóng nếu dữ liệu đã bị phá hủy, sau khi
đầu tiên phá hủy bất kỳ bức ảnh chụp và loại bỏ bất kỳ đặt.</translation>
    </message>
    <message>
        <source>Read-only property that identifies the amount of disk space that is used by a
refreservation set on a dataset, which would be freed if the refreservation was
removed.</source>
        <translation>Thuộc tính chỉ đọc xác định số lượng không gian đĩa được sử dụng bởi một
refreservation đặt trên một bộ dữ liệu, mà sẽ được giải phóng nếu refreservation được
loại bỏ</translation>
    </message>
    <message>
        <source>Read-only property that identifies the amount of disk space that is consumed
by snapshots of a dataset. In particular, it is the amount of disk space that
would be freed if all of this dataset&apos;s snapshots were destroyed. Note that
this value is not simply the sum of the snapshots&apos; used properties, because
space can be shared by multiple snapshots.</source>
        <translation>Thuộc tính chỉ đọc xác định số lượng không gian đĩa được tiêu thụ
bởi những bức ảnh của một tập dữ liệu. Đặc biệt, nó là số tiền của không gian đĩa
sẽ được giải phóng nếu tất cả các bức ảnh chụp của bộ dữ liệu này đã bị phá hủy. Lưu ý rằng
giá trị này không chỉ đơn giản là tổng tài sản sử dụng của các bức ảnh chụp, bởi vì
không gian có thể được chia sẻ bởi nhiều bức ảnh chụp.</translation>
    </message>
    <message>
        <source>Identifies the on-disk version of a file system, which is independent of the
pool version. This property can only be set to a later version that is available
from the supported software release.</source>
        <translation>Xác định các phiên bản trên đĩa của một hệ thống tập tin, đó là độc lập với
phiên bản hồ bơi. Khách sạn này chỉ có thể được thiết lập để một phiên bản sau đó là có sẵn
từ việc phát hành phần mềm hỗ trợ.</translation>
    </message>
    <message>
        <source>For volumes, specifies the logical size of the volume.</source>
        <translation>Đối với khối lượng, chỉ định kích thước hợp lý của khối lượng.</translation>
    </message>
    <message>
        <source>For volumes, specifies the block size of the volume.</source>
        <translation>Đối với khối lượng, xác định kích thước khối của khối lượng.</translation>
    </message>
    <message>
        <source>Indicates whether a dataset has been added to a jail.</source>
        <translation>Biết xem một bộ dữ liệu đã được thêm vào một nhà tù.</translation>
    </message>
    <message>
        <source>This property is not supported on FreeBSD.</source>
        <translation>Khách sạn này không được hỗ trợ trên FreeBSD.</translation>
    </message>
    <message>
        <source>The file system is marked to be destroyed after unmount.</source>
        <translation>Hệ thống tập tin được đánh dấu để bị phá hủy sau khi tháo gắn.</translation>
    </message>
    <message>
        <source>The compression ratio achieved for the referenced space of this
dataset, expressed as a multiplier.</source>
        <translation>Tỉ lệ nén đạt được cho các không gian tham chiếu của
bộ dữ liệu, thể hiện như một số nhân.</translation>
    </message>
    <message>
        <source>The amount of referenced space written to this dataset since the
previous snapshot.</source>
        <translation>Số lượng không gian tham chiếu bằng văn bản cho bộ dữ liệu này kể từ khi
ảnh chụp trước đây.</translation>
    </message>
    <message>
        <source>This property is set to the number of user holds on this snapshot.
</source>
        <translation>Khách sạn này được thiết lập để số lượng người sử dụng nắm giữ trên ảnh chụp này.
</translation>
    </message>
    <message>
        <source>Provide a hint to ZFS about handling of synchronous requests in this
dataset.  If logbias is set to latency (the default), ZFS will use
pool log devices (if configured) to handle the requests at low
latency. If logbias is set to throughput, ZFS will not use configured
pool log devices.  ZFS will instead optimize synchronous operations
for global pool throughput and efficient use of resources.</source>
        <translation>Cung cấp một gợi ý cho ZFS về xử lý yêu cầu đồng bộ trong này
bộ dữ liệu. Nếu logbias được thiết lập để độ trễ (mặc định), ZFS sẽ sử dụng
thiết bị ghi bể bơi (nếu cấu hình) để xử lý các yêu cầu ở mức thấp
độ trễ. Nếu logbias được thiết lập để thông qua, ZFS sẽ không sử dụng cấu hình
thiết bị ghi hồ bơi. ZFS thay vào đó sẽ tối ưu hóa hoạt động đồng bộ
cho toàn cầu thông qua hồ bơi và sử dụng hiệu quả các nguồn lực.</translation>
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
        <translation>Kiểm soát hành vi của các yêu cầu đồng bộ (ví dụ fsync (2),
O_DSYNC). Khách sạn này chấp nhận các giá trị sau:
Tiêu chuẩn này là POSIX quy định hành vi của bảo đảm tất cả
yêu cầu đồng bộ được ghi vào lưu trữ ổn định và
tất cả các thiết bị được xóa để đảm bảo dữ liệu không được lưu trữ bởi
bộ điều khiển thiết bị (đây là mặc định).
luôn luôn Mọi giao dịch hệ thống tập tin được viết và đỏ mặt
trước khi cuộc gọi hệ thống của họ trở lại. Này có một \ n lớnhình phạt hiệu quả.
Vô hiệu hóa những yêu cầu vô hiệu hóa đồng bộ. Giao dịch hệ thống tập tin
chỉ cam kết lưu trữ ổn định theo định kỳ.
tùy chọn sẽ cho hiệu suất cao nhất. Tuy nhiên, nó
là rất nguy hiểm như ZFS sẽ được bỏ qua các đồng bộ
nhu cầu giao dịch truyền tĩnh mạch các ứng dụng như dữ liệu
căn cứ hoặc NFS. Quản trị viên chỉ nên sử dụng
lựa chọn khi các rủi ro được hiểu.</translation>
    </message>
    <message>
        <source>This property cannot be changed after the system is created.</source>
        <translation>Khách sạn này không thể thay đổi sau khi hệ thống được tạo ra.</translation>
    </message>
    <message>
        <source>User Property</source>
        <translation>Sử dụng tài sản</translation>
    </message>
    <message>
        <source>Unknown property or information not available.
Read ZFS manual for more information.</source>
        <translation>Tài sản không rõ hoặc thông tin không có sẵn
.Đọc ZFS dẫn để biết thêm thông tin.</translation>
    </message>
    <message>
        <source>[Invalid]</source>
        <translation>[Hợp lệ]</translation>
    </message>
    <message>
        <source>[Inherit]</source>
        <translation>[Kế thừa]</translation>
    </message>
    <message>
        <source>[Added]</source>
        <translation>[Added]</translation>
    </message>
    <message>
        <source>[Changed]</source>
        <translation>[Thay đổi]</translation>
    </message>
    <message>
        <source>User property</source>
        <translation>Bất động sản sử dụng</translation>
    </message>
    <message>
        <source>Device Menu</source>
        <translation>Thiết bị đơn</translation>
    </message>
    <message>
        <source>Add new user property</source>
        <translation>Thêm tài sản người dùng mới</translation>
    </message>
    <message>
        <source>Delete user property</source>
        <translation>Xóa thuộc tính người dùng</translation>
    </message>
</context>
<context>
    <name>DialogMount</name>
    <message>
        <source>Dialog</source>
        <translation>thoại</translation>
    </message>
    <message>
        <source>Device to mount:</source>
        <translation>Thiết bị để gắn kết:</translation>
    </message>
    <message>
        <source>(Device)</source>
        <translation>(Thiết bị)</translation>
    </message>
    <message>
        <source>Mount location:</source>
        <translation>Gắn kết vị trí:</translation>
    </message>
    <message>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <source>Select Directory</source>
        <translation>Chọn thư mục</translation>
    </message>
</context>
<context>
    <name>DialogName</name>
    <message>
        <source>Dialog</source>
        <translation>Thoại</translation>
    </message>
    <message>
        <source>Pool name:</source>
        <translation>Tên hồ:</translation>
    </message>
    <message>
        <source>Do not mount any filesystems</source>
        <translation>Không gắn kết bất kỳ hệ thống tập tin</translation>
    </message>
    <message>
        <source>Read only pool</source>
        <translation>Chỉ đọc hồ bơi</translation>
    </message>
    <message>
        <source>Import options:</source>
        <translation>Tùy chọn nhập khẩu:</translation>
    </message>
    <message>
        <source>Mount with alternative root:</source>
        <translation>Gắn kết với gốc thay thế:</translation>
    </message>
    <message>
        <source>Mount all filesystems automatically</source>
        <translation>Mount with the original thay thế:</translation>
    </message>
    <message>
        <source>Mounting behavior:</source>
        <translation>Lắp hành vi:</translation>
    </message>
    <message>
        <source>Force operation if error (use with caution)</source>
        <translation>Hoạt động vũ lực nếu lỗi (sử dụng một cách thận trọng)</translation>
    </message>
</context>
<context>
    <name>DialogNewPool</name>
    <message>
        <source>Dialog</source>
        <translation>Thoại</translation>
    </message>
    <message>
        <source>Selected</source>
        <translation>Chọn</translation>
    </message>
    <message>
        <source>Pool name:</source>
        <translation>Tên hồ:</translation>
    </message>
    <message>
        <source>Configuration type:</source>
        <translation>Loại cấu hình:</translation>
    </message>
    <message>
        <source>Striped</source>
        <translation>Sọc</translation>
    </message>
    <message>
        <source>Mirror</source>
        <translation>Gương</translation>
    </message>
    <message>
        <source>Raid-Z (single parity)</source>
        <translation>Raid-Z (chẵn lẻ duy nhất)</translation>
    </message>
    <message>
        <source>Raid-Z2 (double parity)</source>
        <translation>Raid-Z2 (chẵn lẻ đôi)</translation>
    </message>
    <message>
        <source>Raid-Z3 (triple parity)</source>
        <translation>Raid-Z3 (chẵn lẻ ba)</translation>
    </message>
    <message>
        <source>Cache</source>
        <translation>Bộ nhớ cache</translation>
    </message>
    <message>
        <source>Log</source>
        <translation>Đăng nhập</translation>
    </message>
    <message>
        <source>Spare</source>
        <translation>tùng</translation>
    </message>
    <message>
        <source>Error Description Here</source>
        <translation>Lỗi Mô tả đây</translation>
    </message>
    <message>
        <source>Must provide a valid pool name</source>
        <translation>Phải cung cấp tên hồ bơi hợp lệ</translation>
    </message>
    <message>
        <source>Must select exactly %1 devices</source>
        <translation>Phải chọn chính xác %1 thiết bị</translation>
    </message>
    <message>
        <source>Must select at least one device</source>
        <translation>Phải chọn ít nhất một thiết bị</translation>
    </message>
</context>
<context>
    <name>DialogPartition</name>
    <message>
        <source>Add Partition</source>
        <translation>Thêm phân vùng</translation>
    </message>
    <message>
        <source>Add new slice/partition</source>
        <translation>Thêm mới lát/phân vùng</translation>
    </message>
    <message>
        <source>100 GB</source>
        <translation>100 GB</translation>
    </message>
    <message>
        <source>Device:</source>
        <translation>Thiết bị:</translation>
    </message>
    <message>
        <source>ada0</source>
        <translation>ada0</translation>
    </message>
    <message>
        <source>Largest available free block:</source>
        <translation>Lớn nhất khối miễn phí có sẵn:</translation>
    </message>
    <message>
        <source>Total Size:</source>
        <translation>Tổng số lượng:</translation>
    </message>
    <message>
        <source>Current layout:</source>
        <translation>Bố trí hiện tại:</translation>
    </message>
    <message>
        <source>Designation</source>
        <translation>định</translation>
    </message>
    <message>
        <source>Start Sector</source>
        <translation>Bắt đầu ngành</translation>
    </message>
    <message>
        <source>Sector Count</source>
        <translation>Lượt ngành</translation>
    </message>
    <message>
        <source>Size</source>
        <translation>kích thước</translation>
    </message>
    <message>
        <source>Color</source>
        <translation>Màu</translation>
    </message>
    <message>
        <source>New partition Size:</source>
        <translation>Kích thước phân vùng mới:</translation>
    </message>
    <message>
        <source>bytes</source>
        <translation>bytes</translation>
    </message>
    <message>
        <source>kB</source>
        <translation>kB</translation>
    </message>
    <message>
        <source>MB</source>
        <translation>MB</translation>
    </message>
    <message>
        <source>GB</source>
        <translation>GB</translation>
    </message>
    <message>
        <source>TB</source>
        <translation>TB</translation>
    </message>
    <message>
        <source>Partition type:</source>
        <translation>Kiểu phân vùng:</translation>
    </message>
    <message>
        <source>freebsd-zfs</source>
        <translation>freebsd-zfs</translation>
    </message>
    <message>
        <source>freebsd-ufs</source>
        <translation>freebsd-ufs</translation>
    </message>
    <message>
        <source>freebsd</source>
        <translation>freebsd</translation>
    </message>
    <message>
        <source>freebsd-boot</source>
        <translation>freebsd-boot</translation>
    </message>
    <message>
        <source>freebsd-swap</source>
        <translation>freebsd-swap</translation>
    </message>
    <message>
        <source>bios-boot</source>
        <translation>bios-boot</translation>
    </message>
    <message>
        <source>efi</source>
        <translation>efi</translation>
    </message>
    <message>
        <source>Create and initialize a new filesystem</source>
        <translation>Tạo và khởi tạo một hệ thống tập tin mới</translation>
    </message>
    <message>
        <source>** FREE **</source>
        <translation>** FREE **</translation>
    </message>
    <message>
        <source>No free space!</source>
        <translation>Không có không gian miễn phí!</translation>
    </message>
</context>
<context>
    <name>DialogProp</name>
    <message>
        <source>Edit Pool Properties</source>
        <translation>Sửa Pool tính</translation>
    </message>
    <message>
        <source>Property</source>
        <translation>tài sản</translation>
    </message>
    <message>
        <source>Value</source>
        <translation>Giá trị</translation>
    </message>
    <message>
        <source>Total size of the storage pool.</source>
        <translation>Tổng kích thước của vùng lưu trữ.</translation>
    </message>
    <message>
        <source>Percentage of pool space used.</source>
        <translation>Tỷ lệ phần trăm của không gian hồ bơi sử dụng.</translation>
    </message>
    <message>
        <source>Alternate root directory. If set, this directory is prepended to any
mount points within the pool. This can be used when examining an
unknown pool where the mount points cannot be trusted, or in an
alternate boot environment, where the typical paths are not valid.
altroot is not a persistent property. It is valid only while the
system is up.  Setting altroot defaults to using cachefile=none, though
this may be overridden using an explicit setting.</source>
        <translation>Thư mục gốc thay thế. Nếu được thiết lập, thư mục này được thêm vào phía trước để bất kỳ
điểm gắn kết trong hồ bơi. Điều này có thể được sử dụng khi kiểm tra một
hồ bơi không biết nơi mà các điểm gắn kết không thể tin tưởng, hoặc trong một
môi trường khởi động thay thế, nơi những con đường điển hình là không hợp lệ.
altroot không phải là một tài sản liên tục. Nó chỉ có hiệu lực trong khi
hệ thống lên. Thiết lập mặc định altroot để sử dụng cachefile = không, mặc dù
điều này có thể được ghi đè bằng cách sử dụng một thiết lập rõ ràng.</translation>
    </message>
    <message>
        <source>The current health of the pool.</source>
        <translation>Sức khỏe hiện tại của hồ bơi.</translation>
    </message>
    <message>
        <source>A unique identifier for the pool.</source>
        <translation>Một định danh duy nhất cho hồ bơi.</translation>
    </message>
    <message>
        <source>The current on-disk version of the pool. This can be increased, but
never decreased. The preferred method of updating pools is with the
&quot;zpool upgrade&quot; command, though this property can be used when a
specific version is needed for backwards compatibility. This property
can be any number between 1 and the current version reported by
&quot;zpool upgrade -v&quot;.</source>
        <translation>Trên đĩa phiên bản hiện tại của hồ bơi. Điều này có thể được tăng lên, nhưng
không bao giờ giảm. Các phương pháp ưa thích của việc cập nhật hồ là với
&quot;zpool nâng cấp&quot; lệnh, mặc dù tài sản này có thể được sử dụng khi một
phiên bản cụ thể là cần thiết cho tương thích ngược. Này sở hữu
có thể là bất kỳ số từ 1 đến phiên bản hiện tại báo cáo của
&quot;zpool nâng cấp-v&quot;.</translation>
    </message>
    <message>
        <source>Identifies the default bootable dataset for the root pool. This
property is expected to be set mainly by the installation and upgrade
programs.</source>
        <translation>Xác định các dữ liệu khả năng khởi động mặc định cho các hồ bơi gốc.
bất động sản dự kiến ​​sẽ được thiết lập chủ yếu bởi việc cài đặt và nâng cấp
chương trình.</translation>
    </message>
    <message>
        <source>Controls whether a non-privileged user is granted access based on the
dataset permissions defined on the dataset. See zfs(8) for more
information on ZFS delegated administration.</source>
        <translation>Điều khiển một người dùng không có đặc quyền được cấp quyền truy cập dựa trên
cho phép dữ liệu được xác định trên bộ dữ liệu. Xem zfs (8) để biết thêm
thông tin về ZFS giao quản lý.</translation>
    </message>
    <message>
        <source>Controls automatic device replacement. If set to &quot;off&quot;, device
replacement must be initiated by the administrator by using the
&quot;zpool replace&quot; command. If set to &quot;on&quot;, any new device, found in the
same physical location as a device that previously belonged to the
pool, is automatically formatted and replaced. The default behavior
is &quot;off&quot;.  This property can also be referred to by its shortened
column name, &quot;replace&quot;.</source>
        <translation>Kiểm soát thay thế thiết bị tự động. Nếu thiết lập để &quot;tắt&quot;, thiết bị
thay thế phải được bắt đầu bởi người quản trị bằng cách sử dụng
&quot;zpool thay thế&quot; lệnh. Nếu thiết lập để &quot;vào&quot;, bất kỳ thiết bị mới, được tìm thấy trong
cùng một vị trí địa lý như một thiết bị mà trước đây thuộc về
hồ bơi, được tự động định dạng và thay thế. Hành vi mặc định
là &quot;off&quot;. Khách sạn này cũng có thể được gọi bằng của nó
 rút ngắn tên cột, &quot;thay thế&quot;.</translation>
    </message>
    <message>
        <source>Controls whether information about snapshots associated with this
pool is output when &quot;zfs list&quot; is run without the -t option. The
default value is off.</source>
        <translation>Điều khiển nếu thông tin về bức ảnh chụp liên quan với điều này
hồ bơi là đầu ra khi &quot;danh sách zfs&quot; chạy mà không có tùy chọn-t.
giá trị mặc ​​định là tắt.</translation>
    </message>
    <message>
        <source>Controls automatic pool expansion when the underlying LUN is grown.
If set to &quot;on&quot;, the pool will be resized according to the size of the
expanded device. If the device is part of a mirror or raidz then all
devices within that mirror/raidz group must be expanded before the
new space is made available to the pool. The default behavior is
&quot;off&quot;.  This property can also be referred to by its shortened column
name, expand.</source>
        <translation>Điều khiển mở rộng hồ bơi tự động khi LUN cơ bản được trồng
.Nếu thiết lập để &quot;vào&quot;, các hồ bơi sẽ được thay đổi kích thước theo kích thước của
thiết bị mở rộng. Nếu thiết bị là một phần của một chiếc gương hay raidz sau đó tất cả
các thiết bị trong nhóm gương / raidz phải được mở rộng trước khi
không gian mới được tạo sẵn cho hồ bơi. Hành vi mặc định
&quot;off&quot;. Khách sạn này cũng có thể được gọi bằng của nó rút ngắn cột
tên, mở rộng.</translation>
    </message>
    <message>
        <source>Threshold for the number of block ditto copies. If the reference
count for a deduplicated block increases above this number, a new
ditto copy of this block is automatically stored. Default setting is
0.</source>
        <translation>Ngưỡng cho số lượng bản sao khối như trên. Nếu tham chiếu
tính cho một khối deduplicated tăng hơn con số này,
 mới bản sao như trên của khối này được tự động lưu trữ. Thiết lập mặc định
0.</translation>
    </message>
    <message>
        <source>The deduplication ratio specified for a pool, expressed as a multiplier. For
example, a value of 1.76 indicates that 1.76 units of data were stored but
only 1 unit of disk space was actually consumed. See zfs(8) for a description
of the deduplication feature.</source>
        <translation>Tỷ lệ pc chỉ định cho một hồ bơi, thể hiện như một số nhân.
Ví dụ, giá trị 1,76 cho thấy 1,76 đơn vị dữ liệu được lưu trữ nhưng
chỉ có 1 đơn vị của không gian đĩa đã được thực tế tiêu thụ. Xem zfs (8) cho một mô tả
các tính năng chống trùng lắp.</translation>
    </message>
    <message>
        <source>Number of blocks within the pool that are not allocated.</source>
        <translation>Số lượng các khối trong hồ bơi mà không được phân bổ.</translation>
    </message>
    <message>
        <source>Amount of storage space within the pool that has been physically allocated.</source>
        <translation>Số lượng không gian lưu trữ trong hồ bơi mà đã được phân bổ về thể chất.</translation>
    </message>
    <message>
        <source>An arbitrary comment string set by the administrator.</source>
        <translation>Một bình luận chuỗi tùy ý thiết lập bởi người quản trị.</translation>
    </message>
    <message>
        <source>This property has currently no value on FreeBSD.</source>
        <translation>Khách sạn này hiện không có giá trị FreeBSD.</translation>
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
        <translation>Kiểm soát vị trí của nơi cấu hình hồ bơi được lưu trữ.
Phát hiện tất cả các hồ trên hệ thống khởi động đòi hỏi một bản sao cache của
cấu hình dữ liệu được lưu trữ trên hệ thống tập tin gốc. Tất cả các hồ
trong bộ nhớ cache này sẽ được tự động nhập khẩu khi khởi động hệ thống. Một số
môi trường, chẳng hạn như cài đặt và phân nhóm, cần bộ nhớ cache
thông tin ở một vị trí khác nhau để hồ không được tự động
nhập khẩu. Thiết lập thuộc tính này lưu trữ cấu hình bể
ở một vị trí khác nhau mà sau này có thể được nhập khẩu với &quot;zpool nhập khẩu
-c &quot;. Thiết lập nó với giá trị đặc biệt&quot; không &quot;tạo ra một hồ bơi tạm thời
đó là không bao giờ được lưu trữ, và giá trị đặc biệt&apos;&apos; (chuỗi rỗng) sử dụng
vị trí mặc định.</translation>
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
        <translation>Nếu thiết lập trên, hồ bơi sẽ được nhập khẩu trong chế độ chỉ đọc với
hạn chế sau đây:
* Dữ liệu đồng bộ trong nhật ký ý định sẽ không thể truy cập
* Các tính chất của các hồ bơi không thể thay đổi
* Bộ dữ liệu của hồ bơi này chỉ có thể được gắn kết chỉ đọc
* Để ghi vào một hồ bơi chỉ đọc, xuất khẩu và nhập khẩu của các hồ bơi
được yêu cầu.
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
        <translation>Hệ thống tập tin mới</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>tên:</translation>
    </message>
    <message>
        <source>Properties override at creation time:</source>
        <translation>Tính ghi đè lên vào thời điểm sáng tạo:</translation>
    </message>
    <message>
        <source>Prevent auto mount (canmount=noauto)</source>
        <translation>Ngăn chặn tự động mount (canmount = tự động)</translation>
    </message>
    <message>
        <source>Mountpoint</source>
        <translation>điểm láp</translation>
    </message>
    <message>
        <source>none</source>
        <translation>không ai</translation>
    </message>
    <message>
        <source>legacy</source>
        <translation>di sản</translation>
    </message>
    <message>
        <source>[path]</source>
        <translation>[path]</translation>
    </message>
    <message>
        <source>Force UTF-8 only (utf8only=on)</source>
        <translation>Lực lượng UTF-8 chỉ (utf8only = trên)</translation>
    </message>
    <message>
        <source>Unicode normalization =</source>
        <translation>Bình thường hóa unicode =</translation>
    </message>
    <message>
        <source>formD</source>
        <translation>hìnhD</translation>
    </message>
    <message>
        <source>formKCF</source>
        <translation>hìnhKCF</translation>
    </message>
    <message>
        <source>Copies =</source>
        <translation>Bản sao =</translation>
    </message>
    <message>
        <source>Deduplication (dedup=on)</source>
        <translation>Chống trùng lặp (dedup = trên)</translation>
    </message>
    <message>
        <source>Compression =</source>
        <translation>nén =</translation>
    </message>
    <message>
        <source>off</source>
        <translation>off</translation>
    </message>
    <message>
        <source>on</source>
        <translation>on</translation>
    </message>
    <message>
        <source>lzjb</source>
        <translation>lzjb</translation>
    </message>
    <message>
        <source>zle</source>
        <translation>zle</translation>
    </message>
    <message>
        <source>gzip</source>
        <translation>gzip</translation>
    </message>
    <message>
        <source>gzip-1</source>
        <translation>gzip-1</translation>
    </message>
    <message>
        <source>gzip-2</source>
        <translation>gzip-2</translation>
    </message>
    <message>
        <source>gzip-3</source>
        <translation>gzip-3</translation>
    </message>
    <message>
        <source>gzip-4</source>
        <translation>gzip-4</translation>
    </message>
    <message>
        <source>gzip-5</source>
        <translation>gzip-5</translation>
    </message>
    <message>
        <source>gzip-6</source>
        <translation>gzip-6</translation>
    </message>
    <message>
        <source>gzip-7</source>
        <translation>gzip-7</translation>
    </message>
    <message>
        <source>gzip-8</source>
        <translation>gzip-8</translation>
    </message>
    <message>
        <source>gzip-9</source>
        <translation>gzip-9</translation>
    </message>
</context>
<context>
    <name>ZManagerWindow</name>
    <message>
        <source>PCBSD Disk Manager</source>
        <translation>PCBSD Disk Manager</translation>
    </message>
    <message>
        <source>Disks</source>
        <translation>đĩa</translation>
    </message>
    <message>
        <source>Device</source>
        <translation>thiết bị</translation>
    </message>
    <message>
        <source>State</source>
        <translation>Nhà nước</translation>
    </message>
    <message>
        <source>ZFS Pools</source>
        <translation>ZFS Pools</translation>
    </message>
    <message>
        <source>Pool Name</source>
        <translation>Tên Pool</translation>
    </message>
    <message>
        <source>Status:</source>
        <translation>Tình trạng:</translation>
    </message>
    <message>
        <source>TextLabel</source>
        <translation>TextLabel</translation>
    </message>
    <message>
        <source>Clear Error</source>
        <translation>Lỗi rõ ràng</translation>
    </message>
    <message>
        <source>poolname</source>
        <translation>poolname</translation>
    </message>
    <message>
        <source>ZFS Filesystems</source>
        <translation>ZFS hệ tập tin</translation>
    </message>
    <message>
        <source>Filesystems</source>
        <translation>hệ tập tin</translation>
    </message>
    <message>
        <source>Status</source>
        <translation>Tình trạng</translation>
    </message>
    <message>
        <source>Refreshing...</source>
        <translation>Làm mới ...</translation>
    </message>
    <message>
        <source>No pools available, right click to create a new one...</source>
        <translation>Không pools sẵn có, kích chuột phải để tạo ra một cái mới ...</translation>
    </message>
    <message>
        <source> (No media in drive)</source>
        <translation>(Không có phương tiện truyền thông trong ổ đĩa)</translation>
    </message>
    <message>
        <source>Avaliable</source>
        <translation>Sẵn</translation>
    </message>
    <message>
        <source>No disk</source>
        <translation>không có đĩa</translation>
    </message>
    <message>
        <source>ZPool: </source>
        <translation>ZPool:</translation>
    </message>
    <message>
        <source>Sliced</source>
        <translation>Cắt lát</translation>
    </message>
    <message>
        <source>Mounted: </source>
        <translation>gắn kết:</translation>
    </message>
    <message>
        <source>Available</source>
        <translation>Có sẵn</translation>
    </message>
    <message>
        <source>Unmounted</source>
        <translation>không có dán</translation>
    </message>
    <message>
        <source>Partitioned</source>
        <translation>Phân vùng</translation>
    </message>
    <message>
        <source>(Exported)
</source>
        <translation>(Xuất khẩu)
</translation>
    </message>
    <message>
        <source>(Destroyed)
</source>
        <translation>(bị phá hủy)
</translation>
    </message>
    <message>
        <source>Offline</source>
        <translation>Ngoại tuyến</translation>
    </message>
    <message>
        <source>Online</source>
        <translation>trực tuyến</translation>
    </message>
    <message>
        <source>Degraded</source>
        <translation>Bị suy thoái</translation>
    </message>
    <message>
        <source>Faulted</source>
        <translation>bị sự cố</translation>
    </message>
    <message>
        <source>Removed</source>
        <translation>Gỡ bỏ</translation>
    </message>
    <message>
        <source>Unavailable</source>
        <translation>không có sẵn</translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation>Chưa biết</translation>
    </message>
    <message>
        <source>Create new pool</source>
        <translation>Tạo ra hồ bơi mới</translation>
    </message>
    <message>
        <source>Rename pool</source>
        <translation>đổi tên hồ bơi</translation>
    </message>
    <message>
        <source>Destroy pool</source>
        <translation>Tiêu diệt hồ bơi</translation>
    </message>
    <message>
        <source>Add devices...</source>
        <translation>Thêm vào thiết bị...</translation>
    </message>
    <message>
        <source>Add log devices...</source>
        <translation>Thêm vào thiết bị ghi...</translation>
    </message>
    <message>
        <source>Add cache devices...</source>
        <translation>Thêm vào các thiết bị bộ nhớ cache...</translation>
    </message>
    <message>
        <source>Add spare devices...</source>
        <translation>Thêm vào các thiết bị phụ tùng...</translation>
    </message>
    <message>
        <source>Scrub</source>
        <translation>Chà</translation>
    </message>
    <message>
        <source>Export pool</source>
        <translation>Hồ bơi xuất khẩu</translation>
    </message>
    <message>
        <source>Import pool</source>
        <translation>Hồ bơi nhập khẩu</translation>
    </message>
    <message>
        <source>Recover destroyed pool</source>
        <translation>Phục hồi hồ bơi bị phá hủy</translation>
    </message>
    <message>
        <source>Properties...</source>
        <translation>Bất động sản...</translation>
    </message>
    <message>
        <source>Attach (mirror) device...</source>
        <translation>Đính kèm (gương) thiết bị...</translation>
    </message>
    <message>
        <source>Detach from mirror</source>
        <translation>Tách từ gương</translation>
    </message>
    <message>
        <source>Take offline</source>
        <translation>Hãy ẩn</translation>
    </message>
    <message>
        <source>Bring online</source>
        <translation>Mang lại trực tuyến</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation>Loại bỏ</translation>
    </message>
    <message>
        <source>zpool Menu</source>
        <translation>zpool đơn</translation>
    </message>
    <message>
        <source>Device Menu</source>
        <translation>Thiết bị đơn</translation>
    </message>
    <message>
        <source>Unmount</source>
        <translation>Tháo gắn kết</translation>
    </message>
    <message>
        <source>Mount</source>
        <translation>Gắn kết</translation>
    </message>
    <message>
        <source>Create MBR partition table</source>
        <translation>Tạo ra bảng phân vùng MBR</translation>
    </message>
    <message>
        <source>Create GPT partition table</source>
        <translation>Tạo ra bảng phân vùng</translation>
    </message>
    <message>
        <source>Create BSD partition table</source>
        <translation>Tạo ra bảng phân vùng BSD</translation>
    </message>
    <message>
        <source>Delete Partition Table</source>
        <translation>Xóa bảng phân vùng</translation>
    </message>
    <message>
        <source>Add new slice</source>
        <translation>Thêm vào slice mới</translation>
    </message>
    <message>
        <source>Add new partition</source>
        <translation>Thêm vào phân vùng mới</translation>
    </message>
    <message>
        <source>Destroy this slice</source>
        <translation>Tiêu diệt mảnh đất này</translation>
    </message>
    <message>
        <source>Destroy this partition</source>
        <translation>Tiêu diệt phân vùng này</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Warning</translation>
    </message>
    <message>
        <source>This operation cannot be undone.
OK to destroy the slice/partition?</source>
        <translation>Hoạt động này không thể được hoàn tác.
OK để tiêu diệt các slice / phân vùng?</translation>
    </message>
    <message>
        <source>Error report</source>
        <translation>Báo cáo lỗi</translation>
    </message>
    <message>
        <source>Create new zpool</source>
        <translation>Tạo zpool mới</translation>
    </message>
    <message>
        <source>Important information</source>
        <translation>Thông tin quan trọng</translation>
    </message>
    <message>
        <source>The pool was imported in read-only mode, therefore attempting to destroy the pool will leave the pool in the state it was when imported, not necessarily destroyed.</source>
        <translation>Hồ bơi được nhập khẩu trong chế độ chỉ đọc, do đó cố gắng để tiêu diệt các hồ bơi sẽ rời khỏi hồ bơi trong nhà nước đó là khi nhập khẩu, không nhất thiết phải tiêu huỷ.</translation>
    </message>
    <message>
        <source>Attach mirror devices to </source>
        <translation>Đính kèm các thiết bị gương để</translation>
    </message>
    <message>
        <source>The pool was imported in read-only mode, therefore attempting to export the pool will leave the pool in the state it was when imported, not necessarily exported.</source>
        <translation>Hồ bơi được nhập khẩu trong chế độ chỉ đọc, do đó cố gắng để xuất khẩu các hồ bơi sẽ rời khỏi hồ bơi trong nhà nước đó là khi nhập khẩu, không nhất thiết phải xuất khẩu.</translation>
    </message>
    <message>
        <source>Recover destroyed pool as...</source>
        <translation>Phục hồi hồ bơi bị phá hủy như...</translation>
    </message>
    <message>
        <source>Import pool as...</source>
        <translation>Hồ bơi nhập khẩu theo...</translation>
    </message>
    <message>
        <source>The pool was imported in read-only mode, it cannot be renamed.</source>
        <translation>Hồ bơi được nhập khẩu trong chế độ chỉ đọc, nó không thể được đổi tên thành.</translation>
    </message>
    <message>
        <source>Add more devices to zpool</source>
        <translation>Bổ sung thêm thiết bị để zpool</translation>
    </message>
    <message>
        <source>Add log devices to zpool</source>
        <translation>Thêm vào các thiết bị đăng nhập để zpool</translation>
    </message>
    <message>
        <source>Add cache devices to zpool</source>
        <translation>Thêm vào các thiết bị bộ nhớ cache để zpool</translation>
    </message>
    <message>
        <source>Add spare devices to zpool</source>
        <translation>Thêm vào các thiết bị phụ tùng để zpool</translation>
    </message>
    <message>
        <source>[Mounted]</source>
        <translation>[Được lắp]</translation>
    </message>
    <message>
        <source>[Not Mounted]</source>
        <translation>[Không Được lắp]</translation>
    </message>
    <message>
        <source> of </source>
        <translation>của</translation>
    </message>
    <message>
        <source>Rename dataset</source>
        <translation>đổi tên bộ dữ liệu</translation>
    </message>
    <message>
        <source>Create new dataset</source>
        <translation>Tạo bộ dữ liệu mới</translation>
    </message>
    <message>
        <source>Create a clone dataset</source>
        <translation>Tạo ra một bản sao dữ liệu</translation>
    </message>
    <message>
        <source>Destroy dataset</source>
        <translation>Tiêu diệt dữ liệu</translation>
    </message>
    <message>
        <source>Promote filesystem</source>
        <translation>Thúc đẩy hệ thống tập tin</translation>
    </message>
    <message>
        <source>Take a snapshot</source>
        <translation>Có một bản chụp</translation>
    </message>
    <message>
        <source>Destroy snapshot</source>
        <translation>tiêu diệt ảnh chụp</translation>
    </message>
    <message>
        <source>Rollback to this snapshot</source>
        <translation>Rollback đến ảnh chụp này</translation>
    </message>
    <message>
        <source>Edit properties</source>
        <translation>Chỉnh sửa các thuộc tính</translation>
    </message>
    <message>
        <source>zfs Menu</source>
        <translation>zfs đơn</translation>
    </message>
    <message>
        <source>Confirmation</source>
        <translation>Thêm Sức</translation>
    </message>
    <message>
        <source>Take a new snapshot</source>
        <translation>Có một bản chụp mới</translation>
    </message>
    <message>
        <source>New name</source>
        <translation>Tên mới</translation>
    </message>
    <message>
        <source>This operation cannot be undone and will cause all data added after the snapshot to be lost.
Any snapshots created after this one will be deleted, along with any clone filesystems that depend on them.
</source>
        <translation>Hoạt động này không thể hoàn tác và sẽ gây ra tất cả các dữ liệu thêm vào sau khi ảnh chụp bị mất.
Bất kỳ bức ảnh chụp được tạo ra sau khi một này sẽ bị xóa, cùng với bất kỳ hệ thống tập tin bản sao phụ thuộc vào chúng.
</translation>
    </message>
    <message>
        <source>This pool had been destroyed, and its disks may have been reused. Attempting to recover will destroy any new data that could&apos;ve been stored in the devices that were reused and cannot be recovered.
Proceed with recovery?</source>
        <translation>Hồ bơi này đã bị phá hủy, và các ổ đĩa có thể được tái sử dụng. Cố gắng phục hồi sẽ tiêu diệt bất kỳ dữ liệu mới mà có thể đã được lưu trữ trong các thiết bị đã được tái sử dụng và không thể phục hồi.
 Tiến hành thu hồi?</translation>
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
