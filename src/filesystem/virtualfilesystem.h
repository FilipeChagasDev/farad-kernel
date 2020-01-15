/* 
 * --- KERNEL.F SOURCE CODE FILE ---
 * 
 * Author: Filipe Chagas Ferraz
 * Email: filipe.ferraz0@gmail.com
 * GitHub: github.com/filipechagasdev
 * 
 */

#ifndef filesystem_virtualfilesystem_h
#define filesystem_virtualfilesystem_h

#include <filesystem/devicefile.h>
#include <filesystem/filehandler.h>
#include <types.h>

/*
Virtual file system is an abstract interface that generalizes all filesystems in the system.
For each storage device or partition, an instance copy of this interface must be created, defined, and registered.

Definitions:

    FILENAME: Name of a file without the complete path. 
    Example: "file.txt"

    DIRNAME: Name of a directory without the complete path. 
    Example: "documents"
    
    PATH: Path to the directory that contains the file or subdirectory named with the filename or dirname. 
    Example: "/home"
    
    PATH_FILENAME: Complete path to a file. 
    Example: "/home/documents/file.txt"
    
    PATH_DIRNAME: Complete path to a directory. 
    Example: "/home/documents"

    DEVICEFILE: File that is used for kernel streaming. It has no data to write to storage,
    but it can be used as a read-write interface by applications to securely communicate with kernel modules.
    Event callbacks from these files are handled by a devicefile_interface_t.
*/

typedef struct 
{
    // ------ WRITE, READ, OPEN, CLOSE  ------ 

    /** Write buffer data to file **/
    status_t (*write)                  ( file_handler_t *handler, void *buffer, ulong_t buff_size );

    /** Read file data to buffer **/
    status_t (*read)                   ( file_handler_t *handler, void *buffer, ulong_t buff_size );

    /** Load file to handler **/
    status_t (*open)                   ( string_t filename, ulong_t process_id, file_handler_t *handler );

    /** Close file **/
    status_t (*close)                  ( file_handler_t *handler );

    // ------ CREATE, DELETE, MOVE, RENAME ------

    /** Create empty file "path/filename" **/
    status_t (*create_file)            ( string_t path, string_t filename );

    /** Create devicefile "path/filename" **/
    status_t (*create_devicefile)      ( string_t path, string_t filename, devicefile_interface_t *interface );

    /** Create directory "path/dirname/" **/
    status_t (*create_directory)       ( string_t path, string_t dirname );

    /** Delete file **/
    status_t (*delete_file)            ( string_t path_filename );

    /** Delete directory (along with the fully files and directories subtree) **/
    status_t (*delete_directory)       ( string_t path_dirname );

    /** Rename file: "path/filename" to "path/new_filename" **/
    status_t (*rename_file)            ( string_t path, string_t filename, string_t new_filename );

    /** Rename directory: "path/dirname/" to "path/new_dirname/" **/
    status_t (*rename_directory)       ( string_t path, string_t dirname, string_t new_dirname );

    /** Move file: "path/filename" to "new_path/filename" **/
    status_t (*move_file)              ( string_t filename, string_t path, string_t new_path );

    /** Move directory (along with the fully files and directories subtree): "path/dirname" to "new_path/dirname" **/
    status_t (*move_directory)         ( string_t dirname, string_t path, string_t new_path );

    // ------ LIST ------

    /** List filenames (without complete path) in the "path" directory. It creates a heap string array **/
    status_t (*list_directory_files)   ( string_t path, string_array_t *array, ulong_t *array_size );

    /** List subdirectories (without complete path) in the "path" directory. It creates a heap string array **/
    status_t (*list_subdirectories)    ( string_t path, string_array_t *array, ulong_t *array_size );


}virtual_filesystem_interface_t;

#endif //end module