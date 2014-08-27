<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.0">
<context>
    <name>DialogFSProp</name>
    <message>
        <source>Dialog</source>
        <translation>Dialog</translation>
    </message>
    <message>
        <source>Apply changes recursively to all descendants</source>
        <translation>Terapkan perubahan rekursif ke semua bantuan</translation>
    </message>
    <message>
        <source>Property</source>
        <translation>Peralatan</translation>
    </message>
    <message>
        <source>Value</source>
        <translation>Nilai</translation>
    </message>
    <message>
        <source>Source</source>
        <translation>Sumber</translation>
    </message>
    <message>
        <source>Add user property</source>
        <translation>Tambahkan perlengkapan pengguna</translation>
    </message>
    <message>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <source>Remove user property</source>
        <translation>Hapus peralatan pengguna</translation>
    </message>
    <message>
        <source>Inherit from parent</source>
        <translation>Warisan dari orangtua</translation>
    </message>
    <message>
        <source>Revert changes</source>
        <translation>Mengembalikan perubahan</translation>
    </message>
    <message>
        <source>Controls how ACL entries are inherited when files and directories are created. The values are
discard, noallow, restricted, and passthrough. For a description of these values, see ACL Properties.</source>
        <translation>Mengontrol bagaimana isi ACL diwariskan ketika file dan direktori dibuat. Nilai-nilainya adalah \ nmembuang, tidak diijinkan, terbatas, dan melewati. Untuk penjelasan nilai-nilai ini, lihat ACL Properties.</translation>
    </message>
    <message>
        <source>Controls how an ACL entry is modified during a chmod operation.
The values are discard, groupmask, and passthrough. For a description of
these values, see ACL Properties.</source>
        <translation>Kontrol bagaimana sebuah entri ACL diubah selama operasi chmod \ n.Nilai-nilainya adalah membuang, groupmask, dan melewati. Untuk keterangan \ nnilai-nilai ini, lihat ACL Properties.</translation>
    </message>
    <message>
        <source>Controls whether the access time for files is updated when they are read.
Turning this property off avoids producing write traffic when reading files and
can result in significant performance gains, though it might confuse mailers and
similar utilities.</source>
        <translation>Kontrol apakah waktu akses untuk file diperbaharui ketika dibaca.
Matikan peralatan ini untuk menghindari produksi lalu lintas tulisan ketika membaca file.
dapat menghasilkan keuntungan kinerja yang signifikan, meskipun mungkin membingungkan mailer dan
utilitas serupa.</translation>
    </message>
    <message>
        <source>Read-only property that identifies the amount of disk space available to a dataset
and all its children, assuming no other activity in the pool. Because disk space is
shared within a pool, available space can be limited by various factors including
physical pool size, quotas, reservations, and other datasets within the pool.</source>
        <translation>Peralatan Read-only yang mengidentifikasi jumlah ruang disk yang tersedia untuk dataset
dan semua anak-anaknya, dengan asumsi tidak ada kegiatan lain di pool. Karena ruang disk
dibagi dalam pool, ruang yang tersedia dibatasi oleh berbagai faktor termasuk
fisik ukuran pool, kuota, pemesanan, dan dataset lain dalam pool.</translation>
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
        <translation>Jika peralatan ini dimatikan, sistem file tidak bisa ditautkan,
dan diabaikan oleh &apos;zfs mount -a&apos;. Matikan peralatan ini
sama dengan mengatur peralatan titik tautan menjadi nihil, kecuali dataset masih memiliki peralatan titik tautan normal, yang bisa diwariskan
Mematikan peralatan inimengijinkan dataset digunakan hanya untuk
mekanisme peralatan warisan. Salah satu contoh pengaturan canmount = off adalah
memiliki dua dataset dengan mountpoint yang sama, sehingga anak-anak
kedua dataset muncul dalam direktori yang sama, tetapi mungkin memiliki perbedaan
karakteristik yang diwarisi.</translation>
    </message>
    <message>
        <source>Controls the checksum used to verify data integrity. The default
value is on, which automatically selects an appropriate algorithm
(currently, fletcher4, but this may change in future releases). The
value off disables integrity checking on user data. Disabling
checksums is NOT a recommended practice.</source>
        <translation>Mengontrol checksum digunakan untuk memverifikasi integritas data.Default
nilai aktif, yang secara otomatis memilih algoritma 
yang sesuai (Saat ini, fletcher4, tapi ini bisa berubah di masa depan).
Nilai off menonaktifkan pemeriksaan integritas pada data pengguna. Menonaktifkan
checksum TIDAK praktek yang disarankan.</translation>
    </message>
    <message>
        <source>Enables or disables compression for a dataset. The values are on, off, lzjb, zle, gzip,
and gzip-N. Currently, setting this property to lzjb, gzip, or gzip-N has the same
effect as setting this property to on. Enabling compression on a file system with
existing data only compresses new data. Existing data remains uncompressed.</source>
        <translation>Mengaktifkan atau menonaktifkan kompresi untuk dataset. Nilai-nilai yang hidup, mati, lzjb, zle, gzip,
dan gzip-N. Saat ini, pengaturan properti ini untuk lzjb, gzip, atau gzip-N memiliki kesamaan
efek seperti mengaktifkan properti ini. Mengaktifkan kompresi pada sistem file dengan
data yang ada hanya kompres data baru. Data yang ada tetap tidak terkompresi.</translation>
    </message>
    <message>
        <source>Read-only property that identifies the compression ratio achieved for a dataset,
expressed as a multiplier. Compression can be enabled by the zfs set compression=on
dataset command. The value is calculated from the logical size of all files and the
amount of referenced physical data. It includes explicit savings through the use of
the compression property.</source>
        <translation>Peralatan Read-only yang mengidentifikasi rasio kompresi dicapai untuk dataset,
dinyatakan sebagai pengganda. Kompresi dapat diaktifkan dengan kompresi zfs set=on
perintah dataset. Nilai ini dihitung dari ukuran logis dari semua file dan
jumlah data fisik yang direferensikan. Ini mencakup penghematan eksplisit melalui penggunaan
peralatan kompresi.</translation>
    </message>
    <message>
        <source>Sets the number of copies of user data per file system. These copies
are in addition to any pool-level redundancy. Disk space used by
multiple copies of user data is charged to the corresponding file and
dataset, and counts against quotas and reservations. In addition, the
used property is updated when multiple copies are enabled. Consider setting
this property when the file system is created because changing this property
on an existing file system only affects newly written data.</source>
        <translation>Atur jumlah salinan data pengguna per sistem file. Salinan ini
adalah tambahan untuk setiap kelebihan pool-level. Ruang disk digunakan oleh
beberapa salinan data yang dibebankan ke file dan dataset yang sesuai
,dan jumlah dibanding kuota dan reservasi. Lagipula,
peralatan yang digunakan diperbaharui ketika beberapa salinan diaktifkan. Mempertimbangkan pengaturan
peralatan ini ketika sistem file dibuat karena sedang mengganti peralatan ini
pada sistem file yang ada hanya berdampak pada data baru ditulis.</translation>
    </message>
    <message>
        <source>Read-only property that identifies the date and time that a dataset was created.</source>
        <translation>Peralatan Read-Only yang mengidentifikasi tanggal dan waktu dataset telah dibuat.</translation>
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
        <translation>Mengkonfigurasi deduplikasi untuk dataset. Nilai default dimatikan.
deduplikasi standar checksum adalah sha256 (ini dapat berubah dalam
kemudian hari). Ketika dedup diaktifkan, checksum didefinisikan di sini menimpa
peralatan checksum. Mengatur nilai untuk memverifikasi memiliki kesamaan
efek dengan pengaturan sha256, memverifikasi.
Jika diatur untuk memverifikasi, ZFS akan melakukan comparasi byte-to-byte dalam kondisi
dua blok yang memiliki tanda tangan yang sama untuk memastikan isi blok
identik.</translation>
    </message>
    <message>
        <source>The devices property is currently not supported on FreeBSD.</source>
        <translation>Peralatan perangkat tidak didukung pada FreeBSD saat ini.</translation>
    </message>
    <message>
        <source>Controls whether processes can be executed from within this file
system. The default value is on.</source>
        <translation>Kontrol apakah proses bisa dilaksanakan dari dalam sistem file ini
Nilai standar aktif.</translation>
    </message>
    <message>
        <source>Read-only property that indicates whether a file system, clone, or snapshot
is currently mounted.</source>
        <translation>Peralatan Read-only yang mengindikasi diantara sistem file, klon, atau snapshot
saat ini ditautkan.</translation>
    </message>
    <message>
        <source>Controls the mount point used for this file system. When the mountpoint property
is changed for a file system, the file system and any descendents that inherit the
mount point are unmounted. If the new value is legacy, then they remain unmounted.
Otherwise, they are automatically remounted in the new location if the property was
previously legacy or none, or if they were mounted before the property was changed.
In addition, any shared file systems are unshared and shared in the new location.</source>
        <translation>Mengontrol titik tautan digunakan untuk sistem file ini. Ketika properti mountpoint \ nberubah untuk sistem file, sistem file dan setiap bantuan yang mewarisi
titik tautan yang dilepaskan. Jika nilai baru adalah warisan, maka tetap dilepaskan.
Jika tidak, mereka secara otomatis dilepaskan kembali di lokasi baru jika peralatan itu
sebelumnya warisan atau tidak, atau jika mereka dipasang sebelum properti itu berubah.
Selain itu, setiap sistem berbagi file yang unshared dan berbagi di lokasi baru.</translation>
    </message>
    <message>
        <source>Controls what is cached in the primary cache (ARC). Possible values are all,
none, and metadata. If set to all, both user data and metadata are cached. If
set to none, neither user data nor metadata is cached. If set to metadata, only
metadata is cached.</source>
        <translation>Kontrol yang di-cache dalam cache primer (ARC). Nilai yang mungkin adalah semua,
nihil, dan metadata. Jika diatur ke semua, baik data pengguna dan metadata dicache. Jika
diatur none, baik data pengguna atau metadata dicache. Jika diatur ke metadata, hanya
metadata yang dicache.</translation>
    </message>
    <message>
        <source>Read-only property for cloned file systems or volumes that identifies
the snapshot from which the clone was created. The origin cannot be
destroyed (even with the -r or -f option) as long as a clone exists.
Non-cloned file systems have an origin of none.</source>
        <translation>Peralatan Read-Only untuk sistem file yang telah diklon atau volum yang mengidentifikasi
snapshot dari klon yang dibuat. Asal tidak dapat
dihancurkan (bahkan dengan-r atau- opsi f) selama klon ada.
File sistem Non-kloning memiliki asal nihil.</translation>
    </message>
    <message>
        <source>Limits the amount of disk space a dataset and its descendents can consume.
This property enforces a hard limit on the amount of disk space used,
including all space consumed by descendents, such as file systems and snapshots.
Setting a quota on a descendent of a dataset that already has a quota does not
override the ancestor&apos;s quota, but rather imposes an additional limit.</source>
        <translation>Batas jumlah ruang disk dataset dan bantuannya bisa digunakan.
Peralatan ini mendesak batas keras pada jumlah ruang disk yang digunakan,
termasuk semua ruang yang dipakai oleh bantuan, seperti sistem file, dan snapshot.
Pengaturan kuota pada bantuan dataset yang telah memiliki kuota tidak menolak kuota bebet, melainkan memaksa batas tambahan.</translation>
    </message>
    <message>
        <source>Controls whether a dataset can be modified. When set to on,
no modifications can be made.</source>
        <translation>Kontrol apakah dataset bisa dimodifikasi. Ketika diaktifkan,
tidak ada modifikasi yang bisa dibuat.</translation>
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
        <translation>Tentukan ukuran blok untuk file di sistem ini. Peralatan ini
didesain semata-mata untuk digunakan dengan beban kerja database yang mengakses file catatan ukuran tetap. ZFS secara otomatis menyesuaikan ukuran blok
berdasarkan optimalisasi algoritma internal untuk pola akses khusus.
Kegunaan peralatan ini untuk tujuan umum sistem file menghalangi dengan kuat,
dan mungkin memperburuk penampilan dampaknya.
Ukuran yang ditentukan harus sebuah kekuatan yang dua kali lebih besar atau sama dengan 512 dan kurang dari atau sama dengan 128 Kbytes.</translation>
    </message>
    <message>
        <source>Read-only property that identifies the amount of data accessible by a
dataset, which might or might not be shared with other datasets in the
pool.</source>
        <translation>Peralatan Read-only yang mengidentifikasi jumlah data yang bisa diakses dengan
sebuah dataset, yang mungkin atau tidak untuk dibagi dengan dataset lainnya di pool.
</translation>
    </message>
    <message>
        <source>Sets the amount of disk space that a dataset can consume. This property
enforces a hard limit on the amount of space used. This hard limit does
not include disk space used by descendents, such as snapshots and clones.</source>
        <translation>Atur jumlah ruang disk yang bisa digunakan sebuah dataset. Peralatan ini
memberlakukan batas keras pada jumlah ruang yang digunakan. Batas ini tidak
termasuk ruang disk yang digunakan oleh bantuan lainnya, seperti snapshot dan klon.</translation>
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
        <translation>Atur jumlah minimum ruang disk yang dijamin untuk sebuah dataset, tidak
termasuk turunannya, seperti snapshot dan kloningan. Ketika jumlah ruang disk digunakan dibawah nilai ini, dataset dianggap sebagai jumlah ruang yang ditentukan oleh refreservation. Pemesanan Refreservation
dicatat dalam ruang disk dataset induk yang digunakan, dan jumlah terhadap
kuota dataset induk dan pemesanan
.Jika refreservation diatur, snapshot hanya diperbolehkan jika pool memiliki ruang yang cukup
tersedia di luar dari reservasi ini untuk mengakomodasi jumlah
byte saat ini yang direferensikan dalam dataset.</translation>
    </message>
    <message>
        <source>Sets the minimum amount of disk space guaranteed to a dataset and its descendents.
When the amount of disk space used is below this value, the dataset is treated as if
it were using the amount of space specified by its reservation. Reservations are
accounted for in the parent dataset&apos;s disk space used, and count against the parent
dataset&apos;s quotas and reservations.</source>
        <translation>Atur jumlah minimum ruang disk yang dijamin untuk dataset dan turunannya.
Ketika jumlah ruang disk yang digunakan dibawah nilai ini, dataset dianggap sebagaimana
jika ia menggunakan jumlah ruang yang sesuai dengan reservasinya. Reservasinya
dicatat untuk ruang disk yang digunakan dataset induk, dan jumlah terhadap kuota dataset induk
dan reservasi.</translation>
    </message>
    <message>
        <source>Controls what is cached in the secondary cache (L2ARC). Possible values are all, none,
and metadata. If set to all, both user data and metadata are cached. If set to none,
neither user data nor metadata is cached. If set to metadata, only metadata is cached.</source>
        <translation>Mengontrol cache apa yang ada di cache kedua (L2ARC). Nilai yang mungkin adalah semua, tidak ada,
dan metadata. Jika diatur ke semua, kedua data pengguna dan metadata dicache. Jika diatur ke tidak ada,
data pengguna maupun metadata dicache. Jika diatur ke metadata, hanya metadata yang dicache.</translation>
    </message>
    <message>
        <source>Controls whether the setuid bit is honored in a file system.</source>
        <translation>Mengontrol apakah bit setuid dikenal di sistem file.</translation>
    </message>
    <message>
        <source>Controls whether a file system is available over NFS and what
options are used. If set to on, the zfs share command is invoked
with no options. Otherwise, the zfs share command is invoked with
options equivalent to the contents of this property.</source>
        <translation>Mengontrol apakah sistem file tersedia melalui NFS dan apa
opsi yang digunakan. Jika diaktifkan, perintah membagi zfs diminta
tanpa pilihan. Jika tidak, perintah membagi zfs diminta dengan
pilihan yang setara dengan isi peralatan ini.</translation>
    </message>
    <message>
        <source>The sharesmb property has currently no effect on FreeBSD.</source>
        <translation>Peralatan sharesmb saat ini tidak berdampak pada FreeBSD.</translation>
    </message>
    <message>
        <source>Controls whether the .zfs directory is hidden or visible in the root of the file system.</source>
        <translation>Kontrol apakah direktori zfs tersembunyi atau tampak pada root sistem file.</translation>
    </message>
    <message>
        <source>Read-only property that identifies the dataset type as filesystem (file system or clone), volume, or snapshot.</source>
        <translation>Peralatan Read-only yang mengidentifikasi jenis dataset sebagai sistem file (sistem file atau klon), volum, atau snapshot.</translation>
    </message>
    <message>
        <source>Read-only property that identifies the amount of disk space consumed by a dataset and all its descendents.</source>
        <translation>Peralatan Read-only yang mengidentifikasi jumlah ruang disk yang digunakan oleh dataset dan semua tambahannya.</translation>
    </message>
    <message>
        <source>Read-only property that identifies the amount of disk space that is used by
children of this dataset, which would be freed if all the dataset&apos;s children
were destroyed.</source>
        <translation>Peralatan Read-only yang mengidentifikasi jumlah ruang disk yang digunakan oleh
anak-anak dari dataset ini, yang akan membebaskan semua anak-anak dataset
yang dihancurkan.</translation>
    </message>
    <message>
        <source>Read-only property that identifies the amount of disk space that is used by
a dataset itself, which would be freed if the dataset was destroyed, after
first destroying any snapshots and removing any refreservation reservations.</source>
        <translation>Peralatan read-only yang mengidentifikasi jumlah ruang disk yang digunakan oleh
dataset, yang akan dikosongkan jika dataset dihancurkan, setelah
penghancuran pertama setiap snapshot dan menghapus setiap syarat refreservation.</translation>
    </message>
    <message>
        <source>Read-only property that identifies the amount of disk space that is used by a
refreservation set on a dataset, which would be freed if the refreservation was
removed.</source>
        <translation>Peralatan Read-only yang mengidentifikasi jumlah ruang disk yang digunakan oleh
refreservation set pada dataset, yang akan dikosongkan jika syarat-syarat tersebut dihapus.</translation>
    </message>
    <message>
        <source>Read-only property that identifies the amount of disk space that is consumed
by snapshots of a dataset. In particular, it is the amount of disk space that
would be freed if all of this dataset&apos;s snapshots were destroyed. Note that
this value is not simply the sum of the snapshots&apos; used properties, because
space can be shared by multiple snapshots.</source>
        <translation>Peralatan read-only yang mengidentifikasi jumlah ruang disk yang digunakan
oleh snapshot dataset. Khususnya, jumlah ruang disk yang
akan dikosongkan jika semua snapshot dataset dihancurkan. Perlu diperhatikan bahwa
nilai ini tidak hanya jumlah dari peralatan snapshot yang &apos;digunakan, karena
ruang dapat digunakan bersama oleh beberapa snapshot.</translation>
    </message>
    <message>
        <source>Identifies the on-disk version of a file system, which is independent of the
pool version. This property can only be set to a later version that is available
from the supported software release.</source>
        <translation>Mengidentifikasi versi on-disk dari sistem file yang independen dari versi pool. Peralatan ini hanya bisa diatur ke versi lebih baru yang tersedia
dari rilis software yang didukung.</translation>
    </message>
    <message>
        <source>For volumes, specifies the logical size of the volume.</source>
        <translation>Untuk volum, tentukan ukuran volum yang logis.</translation>
    </message>
    <message>
        <source>For volumes, specifies the block size of the volume.</source>
        <translation>Untuk volum, tentukan ukuran blok volum.</translation>
    </message>
    <message>
        <source>Indicates whether a dataset has been added to a jail.</source>
        <translation>Mengindikasi apakah dataset telah ditambahkan ke Jail.</translation>
    </message>
    <message>
        <source>This property is not supported on FreeBSD.</source>
        <translation>Peralatan ini tidak didukung pada FreeBSD.</translation>
    </message>
    <message>
        <source>The file system is marked to be destroyed after unmount.</source>
        <translation>Sistem file ditandai untuk dihancurkan setelah dilepaskan.</translation>
    </message>
    <message>
        <source>The compression ratio achieved for the referenced space of this
dataset, expressed as a multiplier.</source>
        <translation>Rasio kompresi dicapai untuk ruang yang direferensi dari dataset ini, yang disebut pengganda.</translation>
    </message>
    <message>
        <source>The amount of referenced space written to this dataset since the
previous snapshot.</source>
        <translation>Jumlah ruang yang direferensikan ditulis pada dataset ini sejak snapshot sebelumnya
</translation>
    </message>
    <message>
        <source>This property is set to the number of user holds on this snapshot.
</source>
        <translation>Properti ini diatur ke jumlah pengguna yang berpegang pada snapshot ini.
</translation>
    </message>
    <message>
        <source>Provide a hint to ZFS about handling of synchronous requests in this
dataset.  If logbias is set to latency (the default), ZFS will use
pool log devices (if configured) to handle the requests at low
latency. If logbias is set to throughput, ZFS will not use configured
pool log devices.  ZFS will instead optimize synchronous operations
for global pool throughput and efficient use of resources.</source>
        <translation>Sediakan sebuah petunjuk untuk ZFS tentang penanganan permintaan sinkronisasi dalam
dataset ini. Jika logbias diatur ke latency (standar), ZFS akan menggunakan
perangkat log pool (jika dikonfigurasi) untuk menangani permintaan pada latensi rendah
Jika logbias diatur ke throughput, ZFS tidak akan menggunakan
perangkat log pool yang terkonfigurasi. ZFS malah akan mengoptimalkan operasi sinkron
untuk throughput pool global dan efisiensi penggunaan sumber daya.</translation>
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
        <translation>Mengontrol perilaku permintaan sinkron (misalnya fsync(2),
O_DSYNC). Properti ini menerima nilai berikut:
Standar  ini adalah perilaku POSIX tertentu yang memastikan semua
permintaan sinkron ditulis ke penyimpanan yang stabil dan
semua perangkat yang memerah untuk memastikan data tidak di-cache oleh
pengendali perangkat (ini adalah bawaan)
.selalu    Semua transaksi sistem file ditulis dan diperah
sebelum panggilan sistem mereka kembali. Ini memiliki 
hukuman kinerja yang besar
.dinonaktifkan  Menonaktifkan permintaan sinkron. Transaksi sistem file
hanya berkomitmen untuk penyimpanan stabil secara berkala.
opsi ini akan memberikan kinerja tertinggi. Namun,
sangat berbahaya karena ZFS akan mengabaikan permintaan
transaksi sinkron aplikasi seperti data-
base atau NFS. Administrator hanya harus menggunakan opsi ini
ketika semua risiko telah dipahami.</translation>
    </message>
    <message>
        <source>This property cannot be changed after the system is created.</source>
        <translation>Peralatan ini tidak bisa diubah setelah sistem dibuat.</translation>
    </message>
    <message>
        <source>User Property</source>
        <translation>Peralatan Pengguna</translation>
    </message>
    <message>
        <source>Unknown property or information not available.
Read ZFS manual for more information.</source>
        <translation>Peralatan tidak dikenal atau informasi tidak tersedia.
Baca manual ZFS untuk informasi lainnya.</translation>
    </message>
    <message>
        <source>[Invalid]</source>
        <translation>[Tidak Valid]</translation>
    </message>
    <message>
        <source>[Inherit]</source>
        <translation>[Keturunan]</translation>
    </message>
    <message>
        <source>[Added]</source>
        <translation>[Ditambahkan]</translation>
    </message>
    <message>
        <source>[Changed]</source>
        <translation>[Diubah]</translation>
    </message>
    <message>
        <source>User property</source>
        <translation>Peralatan Pengguna</translation>
    </message>
    <message>
        <source>Device Menu</source>
        <translation>Menu perangkat</translation>
    </message>
    <message>
        <source>Add new user property</source>
        <translation>Tambahkan peralatan pengguna baru</translation>
    </message>
    <message>
        <source>Delete user property</source>
        <translation>Hapus peralatan pengguna</translation>
    </message>
</context>
<context>
    <name>DialogMount</name>
    <message>
        <source>Dialog</source>
        <translation>Dialog</translation>
    </message>
    <message>
        <source>Device to mount:</source>
        <translation>Perangkat untuk tautan:</translation>
    </message>
    <message>
        <source>(Device)</source>
        <translation>(Perangkat)</translation>
    </message>
    <message>
        <source>Mount location:</source>
        <translation>Lokasi tautan:</translation>
    </message>
    <message>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <source>Select Directory</source>
        <translation>Pilih direktori</translation>
    </message>
</context>
<context>
    <name>DialogName</name>
    <message>
        <source>Dialog</source>
        <translation>Dialog</translation>
    </message>
    <message>
        <source>Pool name:</source>
        <translation>Nama Pool:</translation>
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
        <translation>Dialog</translation>
    </message>
    <message>
        <source>Selected</source>
        <translation>Dipilih</translation>
    </message>
    <message>
        <source>Pool name:</source>
        <translation>Nama Pool:</translation>
    </message>
    <message>
        <source>Configuration type:</source>
        <translation>Jenis Konfigurasi:</translation>
    </message>
    <message>
        <source>Striped</source>
        <translation>Berjalur</translation>
    </message>
    <message>
        <source>Mirror</source>
        <translation>Mirror</translation>
    </message>
    <message>
        <source>Raid-Z (single parity)</source>
        <translation>Raid-Z (persamaan tunggal)</translation>
    </message>
    <message>
        <source>Raid-Z2 (double parity)</source>
        <translation>Raid-Z2 (persamaan ganda)</translation>
    </message>
    <message>
        <source>Raid-Z3 (triple parity)</source>
        <translation>Raid-Z3 (persamaan rangkap tiga)</translation>
    </message>
    <message>
        <source>Cache</source>
        <translation>Cache</translation>
    </message>
    <message>
        <source>Log</source>
        <translation>Log</translation>
    </message>
    <message>
        <source>Spare</source>
        <translation>Cadangan</translation>
    </message>
    <message>
        <source>Error Description Here</source>
        <translation>Kesalahan Deskripsi Disini</translation>
    </message>
    <message>
        <source>Must provide a valid pool name</source>
        <translation>Harus menyediakan nama pool yang valid</translation>
    </message>
    <message>
        <source>Must select exactly %1 devices</source>
        <translation>Harus memilih satu perangkat</translation>
    </message>
    <message>
        <source>Must select at least one device</source>
        <translation>Harus memilih setidaknya satu perangkat</translation>
    </message>
</context>
<context>
    <name>DialogPartition</name>
    <message>
        <source>Add Partition</source>
        <translation>Tambah Partisi</translation>
    </message>
    <message>
        <source>Add new slice/partition</source>
        <translation>Tambah slice/partisi baru</translation>
    </message>
    <message>
        <source>100 GB</source>
        <translation>100 GB</translation>
    </message>
    <message>
        <source>Device:</source>
        <translation>Perangkat:</translation>
    </message>
    <message>
        <source>ada0</source>
        <translation>ada0</translation>
    </message>
    <message>
        <source>Largest available free block:</source>
        <translation>Blok bebas terbesar yang tersedia:</translation>
    </message>
    <message>
        <source>Total Size:</source>
        <translation>Ukuran total:</translation>
    </message>
    <message>
        <source>Current layout:</source>
        <translation>Layout saat ini:</translation>
    </message>
    <message>
        <source>Designation</source>
        <translation>Penetapan</translation>
    </message>
    <message>
        <source>Start Sector</source>
        <translation>Mulai Sektor</translation>
    </message>
    <message>
        <source>Sector Count</source>
        <translation>Hitungan sektor</translation>
    </message>
    <message>
        <source>Size</source>
        <translation>Ukuran</translation>
    </message>
    <message>
        <source>Color</source>
        <translation>Warna</translation>
    </message>
    <message>
        <source>New partition Size:</source>
        <translation>Ukuran partisi baru:</translation>
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
        <translation>Jenis partisi:</translation>
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
        <translation>Membuat dan menginisialisasikan sistem file baru</translation>
    </message>
    <message>
        <source>** FREE **</source>
        <translation>** BEBAS **</translation>
    </message>
    <message>
        <source>No free space!</source>
        <translation>Tidak ada ruang!</translation>
    </message>
</context>
<context>
    <name>DialogProp</name>
    <message>
        <source>Edit Pool Properties</source>
        <translation>Ubah peralatan pool</translation>
    </message>
    <message>
        <source>Property</source>
        <translation>Peralatan</translation>
    </message>
    <message>
        <source>Value</source>
        <translation>Nilai</translation>
    </message>
    <message>
        <source>Total size of the storage pool.</source>
        <translation>Ukuran total tempat penyimpanan pool.</translation>
    </message>
    <message>
        <source>Percentage of pool space used.</source>
        <translation>Persentase ruang pool digunakan.</translation>
    </message>
    <message>
        <source>Alternate root directory. If set, this directory is prepended to any
mount points within the pool. This can be used when examining an
unknown pool where the mount points cannot be trusted, or in an
alternate boot environment, where the typical paths are not valid.
altroot is not a persistent property. It is valid only while the
system is up.  Setting altroot defaults to using cachefile=none, though
this may be overridden using an explicit setting.</source>
        <translation>Direktori root alternatif. Jika diatur, direktori ini ditambahkan ke setiap
titik tautan dalam pool. Ini bisa digunakan ketika menguji
pool yang tidak diketahui dimana titik tautan tidak bisa dipercaya, atau di
lingkungan boot alternatif, saat path tertentu tidak sah.
altroot bukanlah peralatan yang persisten. Hanya valid selama
sistem naik. Atur altroot bawaan untuk menggunakan cachefile=none, meskipun
ini dapat diganti dengan pengaturan eksplisit.</translation>
    </message>
    <message>
        <source>The current health of the pool.</source>
        <translation>Kesehatan pool saat ini.</translation>
    </message>
    <message>
        <source>A unique identifier for the pool.</source>
        <translation>Pendeteksi unik untuk pool.</translation>
    </message>
    <message>
        <source>The current on-disk version of the pool. This can be increased, but
never decreased. The preferred method of updating pools is with the
&quot;zpool upgrade&quot; command, though this property can be used when a
specific version is needed for backwards compatibility. This property
can be any number between 1 and the current version reported by
&quot;zpool upgrade -v&quot;.</source>
        <translation>Versi on-disk pool saat ini. Hal ini bisa ditingkatkan, tapi
tidak pernah bisa diturunkan. Metode yang lebih baik dalam memperbaharui pool adalah dengan
perintah &quot;zpool upgrade&quot;, meskipun peralatan ini bisa digunakan ketika
versi khusus dibutuhkan untuk kompatibilitas dengan yang sebelumnya. Peralatan ini
bisa jadi adalah setiap angka antara 1 dan versi saat ini yang dilaporkan oleh
&quot;zpool upgrade -v&quot;.</translation>
    </message>
    <message>
        <source>Identifies the default bootable dataset for the root pool. This
property is expected to be set mainly by the installation and upgrade
programs.</source>
        <translation>Mengidentifikasi dataset bawaan yang bisa dibooting untuk root pool. Peralatan ini
diharapkan akan diatur terutama melalui instalasi dan upgrade
program.</translation>
    </message>
    <message>
        <source>Controls whether a non-privileged user is granted access based on the
dataset permissions defined on the dataset. See zfs(8) for more
information on ZFS delegated administration.</source>
        <translation>Kontrol apakah pengguna non-hak istimewa diberikan akses didasarkan pada
izin dataset yang didefinisikan pada dataset. Lihat zfs (8) untuk
informasi selanjutnya tentang administrasi ZFS didelegasikan.</translation>
    </message>
    <message>
        <source>Controls automatic device replacement. If set to &quot;off&quot;, device
replacement must be initiated by the administrator by using the
&quot;zpool replace&quot; command. If set to &quot;on&quot;, any new device, found in the
same physical location as a device that previously belonged to the
pool, is automatically formatted and replaced. The default behavior
is &quot;off&quot;.  This property can also be referred to by its shortened
column name, &quot;replace&quot;.</source>
        <translation>Kontrol penggantian perangkat otomatis. Jika diatur ke &quot;off&quot;, perangkat
pengganti harus diprakarsai oleh administrator dengan menggunakan
perintah &quot;ganti zpool &quot;. Jika diatur ke &quot;on&quot;, perangkat baru, ditemukan dalam
lokasi fisik yang sama dengan perangkat sebelumnya milik
pool, secara otomatis diformat dan diganti. Sifat bawaan
adalah &quot;off&quot;.  Peralatan ini juga dapat disebut dengan nama kolomnya yang disingkat
, &quot;ganti&quot;.</translation>
    </message>
    <message>
        <source>Controls whether information about snapshots associated with this
pool is output when &quot;zfs list&quot; is run without the -t option. The
default value is off.</source>
        <translation>Kontrol apakah informasi tentang snapshot terkait dengan pool ini
adalah output ketika &quot;daftar zfs&quot; dijalankan tanpa opsi -t.
nilai bawaan tidak aktif.</translation>
    </message>
    <message>
        <source>Controls automatic pool expansion when the underlying LUN is grown.
If set to &quot;on&quot;, the pool will be resized according to the size of the
expanded device. If the device is part of a mirror or raidz then all
devices within that mirror/raidz group must be expanded before the
new space is made available to the pool. The default behavior is
&quot;off&quot;.  This property can also be referred to by its shortened column
name, expand.</source>
        <translation>Kontrol ekspansi pool otomatis ketika LUN yang mendasari tumbuh.
Jika diatur ke &quot;on&quot;, pool akan diubah ukurannya sesuai dengan ukuran
perangkat yang diperluas. Jika perangkat adalah bagian dari mirror atau raidz maka semua
perangkat dalam kelompok mirror/raidz harus diperluas sebelum 
ruang baru tersedia untuk pool. Sifat bawaan adalah
&quot;off&quot;. Peralatan ini juga dapat disebut dengan yang disingkat
, memperluas.</translation>
    </message>
    <message>
        <source>Threshold for the number of block ditto copies. If the reference
count for a deduplicated block increases above this number, a new
ditto copy of this block is automatically stored. Default setting is
0.</source>
        <translation>Ambang batas untuk jumlah salinan blok ditto. Jika referensi
menghitung deduplikasi blok meningkat di atas angka ini,
salinan ditto baru blok ini secara otomatis disimpan. Pengaturan standar adalah
0.</translation>
    </message>
    <message>
        <source>The deduplication ratio specified for a pool, expressed as a multiplier. For
example, a value of 1.76 indicates that 1.76 units of data were stored but
only 1 unit of disk space was actually consumed. See zfs(8) for a description
of the deduplication feature.</source>
        <translation>Rasio deduplikasi yang ditentukan untuk pool, dinyatakan sebagai pengganda. Untuk
contoh, nilai 1.76 menunjukkan bahwa 1.76 unit data disimpan tetapi
hanya 1 unit ruang disk yang benar-benar terpakai. Lihat zfs(8) untuk penjelasan
fitur deduplikasi ini.</translation>
    </message>
    <message>
        <source>Number of blocks within the pool that are not allocated.</source>
        <translation>Jumlah blok dalam pool yang tidak disediakan.</translation>
    </message>
    <message>
        <source>Amount of storage space within the pool that has been physically allocated.</source>
        <translation>Jumlah ruang penyimpanan dalam pool yang telah ditempatkan.</translation>
    </message>
    <message>
        <source>An arbitrary comment string set by the administrator.</source>
        <translation>Deretan komentar yang diatur oleh administrator.</translation>
    </message>
    <message>
        <source>This property has currently no value on FreeBSD.</source>
        <translation>Peralatan ini tidak ada nilai saat ini di FreeBSD.</translation>
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
        <translation>Sistem file baru</translation>
    </message>
    <message>
        <source>Name:</source>
        <translation>Nama:</translation>
    </message>
    <message>
        <source>Properties override at creation time:</source>
        <translation>Peralatan diganti pada saat pembuatan:</translation>
    </message>
    <message>
        <source>Prevent auto mount (canmount=noauto)</source>
        <translation>Mencegah tautan otomatis (canmount=noauto)</translation>
    </message>
    <message>
        <source>Mountpoint</source>
        <translation>Titik tautan</translation>
    </message>
    <message>
        <source>none</source>
        <translation>nihil</translation>
    </message>
    <message>
        <source>legacy</source>
        <translation>warisan</translation>
    </message>
    <message>
        <source>[path]</source>
        <translation>[path]</translation>
    </message>
    <message>
        <source>Force UTF-8 only (utf8only=on)</source>
        <translation>Hanya daya UTF-8 (utf8only=on)</translation>
    </message>
    <message>
        <source>Unicode normalization =</source>
        <translation>Normalisasi Unicode =</translation>
    </message>
    <message>
        <source>formD</source>
        <translation>formD</translation>
    </message>
    <message>
        <source>formKCF</source>
        <translation>formKCF</translation>
    </message>
    <message>
        <source>Copies =</source>
        <translation>Salinan =</translation>
    </message>
    <message>
        <source>Deduplication (dedup=on)</source>
        <translation>Deduplikasi (dedup=on)</translation>
    </message>
    <message>
        <source>Compression =</source>
        <translation>Penekanan =</translation>
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
        <translation>Pengelola disk PCBSD</translation>
    </message>
    <message>
        <source>Disks</source>
        <translation>Disk</translation>
    </message>
    <message>
        <source>Device</source>
        <translation>Perangkat</translation>
    </message>
    <message>
        <source>State</source>
        <translation>Keadaan</translation>
    </message>
    <message>
        <source>ZFS Pools</source>
        <translation>Pool ZFS</translation>
    </message>
    <message>
        <source>Pool Name</source>
        <translation>Nama Pool</translation>
    </message>
    <message>
        <source>Status:</source>
        <translation>Status:</translation>
    </message>
    <message>
        <source>TextLabel</source>
        <translation>TextLabel</translation>
    </message>
    <message>
        <source>Clear Error</source>
        <translation>Hapus kesalahan</translation>
    </message>
    <message>
        <source>poolname</source>
        <translation>nama pool</translation>
    </message>
    <message>
        <source>ZFS Filesystems</source>
        <translation>Sistem file ZFS</translation>
    </message>
    <message>
        <source>Filesystems</source>
        <translation>Sistem file</translation>
    </message>
    <message>
        <source>Status</source>
        <translation>Status</translation>
    </message>
    <message>
        <source>Refreshing...</source>
        <translation>Sedang menyegarkan...</translation>
    </message>
    <message>
        <source>No pools available, right click to create a new one...</source>
        <translation>Tidak ada pool tersedia, klik kanan untuk buat baru...</translation>
    </message>
    <message>
        <source> (No media in drive)</source>
        <translation>(Tidak ada media dalam perangkat)</translation>
    </message>
    <message>
        <source>Avaliable</source>
        <translation>Tersedia</translation>
    </message>
    <message>
        <source>No disk</source>
        <translation>Tidak ada disk</translation>
    </message>
    <message>
        <source>ZPool: </source>
        <translation>ZPool:</translation>
    </message>
    <message>
        <source>Sliced</source>
        <translation>Diiris</translation>
    </message>
    <message>
        <source>Mounted: </source>
        <translation>Ditautkan:</translation>
    </message>
    <message>
        <source>Available</source>
        <translation>Tersedia</translation>
    </message>
    <message>
        <source>Unmounted</source>
        <translation>Lepas tautan</translation>
    </message>
    <message>
        <source>Partitioned</source>
        <translation>Dipartisi</translation>
    </message>
    <message>
        <source>(Exported)
</source>
        <translation>(Diekspor)
</translation>
    </message>
    <message>
        <source>(Destroyed)
</source>
        <translation>(dihancurkan)
</translation>
    </message>
    <message>
        <source>Offline</source>
        <translation>Offline</translation>
    </message>
    <message>
        <source>Online</source>
        <translation>Online</translation>
    </message>
    <message>
        <source>Degraded</source>
        <translation>Diturunkan</translation>
    </message>
    <message>
        <source>Faulted</source>
        <translation>Disalahkan</translation>
    </message>
    <message>
        <source>Removed</source>
        <translation>Dihapus</translation>
    </message>
    <message>
        <source>Unavailable</source>
        <translation>Tidak tersedia</translation>
    </message>
    <message>
        <source>Unknown</source>
        <translation>Tidak diketahui</translation>
    </message>
    <message>
        <source>Create new pool</source>
        <translation>Buat pool baru</translation>
    </message>
    <message>
        <source>Rename pool</source>
        <translation>Beri nama pool</translation>
    </message>
    <message>
        <source>Destroy pool</source>
        <translation>Hancurkan pool</translation>
    </message>
    <message>
        <source>Add devices...</source>
        <translation>Tambah perangkat...</translation>
    </message>
    <message>
        <source>Add log devices...</source>
        <translation>Tambah perangkat log...</translation>
    </message>
    <message>
        <source>Add cache devices...</source>
        <translation>Tambah perangkat cache...</translation>
    </message>
    <message>
        <source>Add spare devices...</source>
        <translation>Tambah perangkat cadangan...</translation>
    </message>
    <message>
        <source>Scrub</source>
        <translation>Scrub</translation>
    </message>
    <message>
        <source>Export pool</source>
        <translation>Ekspor pool</translation>
    </message>
    <message>
        <source>Import pool</source>
        <translation>Impor pool</translation>
    </message>
    <message>
        <source>Recover destroyed pool</source>
        <translation>Pulihkan pool yang dihancurkan</translation>
    </message>
    <message>
        <source>Properties...</source>
        <translation>Perlengkapan...</translation>
    </message>
    <message>
        <source>Attach (mirror) device...</source>
        <translation>Lampirkan perangkat (mirror)...</translation>
    </message>
    <message>
        <source>Detach from mirror</source>
        <translation>Lepaskan dari mirror</translation>
    </message>
    <message>
        <source>Take offline</source>
        <translation>Offline</translation>
    </message>
    <message>
        <source>Bring online</source>
        <translation>Online</translation>
    </message>
    <message>
        <source>Remove</source>
        <translation>Hapus</translation>
    </message>
    <message>
        <source>zpool Menu</source>
        <translation>Menu zpool</translation>
    </message>
    <message>
        <source>Device Menu</source>
        <translation>Menu perangkat</translation>
    </message>
    <message>
        <source>Unmount</source>
        <translation>Lepas tautan</translation>
    </message>
    <message>
        <source>Mount</source>
        <translation>Tautan</translation>
    </message>
    <message>
        <source>Create MBR partition table</source>
        <translation>Buat meja partisi MBR</translation>
    </message>
    <message>
        <source>Create GPT partition table</source>
        <translation>Buat meja partisi GPT</translation>
    </message>
    <message>
        <source>Create BSD partition table</source>
        <translation>Buat meja partisi BSD</translation>
    </message>
    <message>
        <source>Delete Partition Table</source>
        <translation>Hapus Meja Partisi</translation>
    </message>
    <message>
        <source>Add new slice</source>
        <translation>Tambahkan slice baru</translation>
    </message>
    <message>
        <source>Add new partition</source>
        <translation>Tambahkan partisi baru</translation>
    </message>
    <message>
        <source>Destroy this slice</source>
        <translation>Hancurkan slice ini</translation>
    </message>
    <message>
        <source>Destroy this partition</source>
        <translation>Hancurkan partisi ini</translation>
    </message>
    <message>
        <source>Warning</source>
        <translation>Peringatan</translation>
    </message>
    <message>
        <source>This operation cannot be undone.
OK to destroy the slice/partition?</source>
        <translation>Operasi ini tidak bisa diselesaikan.
OK untuk menghancurkan slice/partisi?</translation>
    </message>
    <message>
        <source>Error report</source>
        <translation>Laporan Kesalahan</translation>
    </message>
    <message>
        <source>Create new zpool</source>
        <translation>Buat zpool baru</translation>
    </message>
    <message>
        <source>Important information</source>
        <translation>Informasi Penting</translation>
    </message>
    <message>
        <source>The pool was imported in read-only mode, therefore attempting to destroy the pool will leave the pool in the state it was when imported, not necessarily destroyed.</source>
        <translation>Pool diimpor dalam mode read-only, karena jika dihancurkan pool akan dalam keadaan ketika pool diimpor, tidak perlu dihancurkan.</translation>
    </message>
    <message>
        <source>Attach mirror devices to </source>
        <translation>Lampirkan perangkar mirror ke </translation>
    </message>
    <message>
        <source>The pool was imported in read-only mode, therefore attempting to export the pool will leave the pool in the state it was when imported, not necessarily exported.</source>
        <translation>Pool diimpor dalam mode read-only, karena jika diekspor pool akan dalam keadaan ketika pool diimpor, tidak perlu diekspor.</translation>
    </message>
    <message>
        <source>Recover destroyed pool as...</source>
        <translation>Pulihkan pool yang dihancurkan sebagai...</translation>
    </message>
    <message>
        <source>Import pool as...</source>
        <translation>Impor pool sebagai...</translation>
    </message>
    <message>
        <source>The pool was imported in read-only mode, it cannot be renamed.</source>
        <translation>Pool diimpor dalam read-only mode, sehingga tidak bisa diberi nama.</translation>
    </message>
    <message>
        <source>Add more devices to zpool</source>
        <translation>Tambahkan perangkat lainnya ke zpool</translation>
    </message>
    <message>
        <source>Add log devices to zpool</source>
        <translation>Tambahkan perangkat log ke zpool</translation>
    </message>
    <message>
        <source>Add cache devices to zpool</source>
        <translation>Tambahkan perangkat cache ke zpool</translation>
    </message>
    <message>
        <source>Add spare devices to zpool</source>
        <translation>Tambahkan perangkat cadangan ke zpool</translation>
    </message>
    <message>
        <source>[Mounted]</source>
        <translation>[Ditautkan]</translation>
    </message>
    <message>
        <source>[Not Mounted]</source>
        <translation>[Tidak Ditautkan]</translation>
    </message>
    <message>
        <source> of </source>
        <translation> dari </translation>
    </message>
    <message>
        <source>Rename dataset</source>
        <translation>Beri nama dataset</translation>
    </message>
    <message>
        <source>Create new dataset</source>
        <translation>Buat dataset baru</translation>
    </message>
    <message>
        <source>Create a clone dataset</source>
        <translation>Buat dataset klon</translation>
    </message>
    <message>
        <source>Destroy dataset</source>
        <translation>Musnahkan dataset</translation>
    </message>
    <message>
        <source>Promote filesystem</source>
        <translation>Meningkatkan sistem file</translation>
    </message>
    <message>
        <source>Take a snapshot</source>
        <translation>Ambil snapshot</translation>
    </message>
    <message>
        <source>Destroy snapshot</source>
        <translation>Musnahkan snapshot</translation>
    </message>
    <message>
        <source>Rollback to this snapshot</source>
        <translation>Putar balik ke snapshot ini</translation>
    </message>
    <message>
        <source>Edit properties</source>
        <translation>Ubah peralatan</translation>
    </message>
    <message>
        <source>zfs Menu</source>
        <translation>Menu zfs</translation>
    </message>
    <message>
        <source>Confirmation</source>
        <translation>Konfirmasi</translation>
    </message>
    <message>
        <source>Take a new snapshot</source>
        <translation>Ambil sebuah snapshot baru</translation>
    </message>
    <message>
        <source>New name</source>
        <translation>Nama Baru</translation>
    </message>
    <message>
        <source>This operation cannot be undone and will cause all data added after the snapshot to be lost.
Any snapshots created after this one will be deleted, along with any clone filesystems that depend on them.
</source>
        <translation>Operasi ini tidak bisa diselesaikan dan akan menyebabkan semua data yang ditambahkan setelah snapshot akan hilang.
Setiap snapshot yang dibuat setelah ini akan dihapus, bersamaan dengan sistem klon yang tergantung pada snapshot tersebut.
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
