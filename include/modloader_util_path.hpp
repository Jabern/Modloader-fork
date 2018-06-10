/* 
 * San Andreas Mod Loader Utilities Headers
 * Created by LINK/2012 <dma_2012@hotmail.com>
 * 
 *  This file provides helpful functions for plugins creators.
 * 
 *  This source code is offered for use in the public domain. You may
 *  use, modify or distribute it freely.
 *
 *  This code is distributed in the hope that it will be useful but
 *  WITHOUT ANY WARRANTY. ALL WARRANTIES, EXPRESS OR IMPLIED ARE HEREBY
 *  DISCLAIMED. This includes but is not limited to warranties of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * 
 */
#ifndef MODLOADER_UTIL_PATH_HPP
#define	MODLOADER_UTIL_PATH_HPP

#include <string>
#include <cstring>
#include <windows.h>
#include <modloader.hpp>
#include <modloader_util_container.hpp>

namespace modloader
{
    static const char* szNullFile = "NUL";          // "/dev/null" on POSIX systems
    static const char cNormalizedSlash = '\\';      // The slash used in the normalized path
    
    /*
     *  MakeSureStringIsDirectory
     *      Makes sure the string @dir is a directory path. If @touchEmpty is true,
     *      dir will be made a directory even when the string is empty.
     * 
     *      A string is considered as a path when it ends with a slash
     */
    inline std::string& MakeSureStringIsDirectory(std::string& dir, bool touchEmpty = true)
    {
        if(dir.empty())
        {
            if(touchEmpty) dir = ".\\";
        }
        else if(dir.back() != cNormalizedSlash)
        {
            dir.push_back(cNormalizedSlash);
        }
        return dir;
    }
    
    
    /*
     *  NormalizePath
     *      Normalizates a path string, so for example:
     *          "somefolder/something" will output "somefolder\\something"
     *          "SOMEfoldER/something/" will output "somefolder\\something"
     *          "somefolder\\something" will output "somefolder\\something"
     *          etc
     */
    inline std::string NormalizePath(std::string path)
    {
        
        std::replace(path.begin(), path.end(), '/', '\\');  // Replace all '/' with '\\'
        tolower(path);                                      // tolower the strings (Windows paths are case insensitive)
        while(path.back() == '/' || path.back() == '\\')    // We don't want a slash at the end of the folder path
            path.pop_back();                                // ..
        trim(path);                                         // Trim the string...
        return path;
    }
    
    
    /*
     *  GetProperlyPath
     *      This works exactly the same as NormalizePath (see above), except if @transform is not a null pointer
     *      It will return the first time that @transform (probably a folder)  appears in the normalized path
     * 
     *      PS: @transform MUST be normalized!
     */
    static std::string GetProperlyPath(std::string path, const char* transform)
    {
        path = NormalizePath(std::move(path));
        if(transform)
        {
            size_t pos = path.find(transform);
            if(pos != 0 && pos != path.npos) path.erase(0, pos);
        }
        return path;
    }

    /*
     * ForeachFile
     *      Iterates on all files in a directory, files beggining with '.' will be ignored.
     *      @dir: Directory to search at
     *      @mask: Search mask
     *      @bRecursive: Recursive search?
     *      @cb: Callback to call on each file (bool(ModLoaderFile& file))
     */
    template<class T>
    inline bool ForeachFile(std::string dir, std::string mask, bool bRecursive, T cb)
    {
        HANDLE hSearch;
        WIN32_FIND_DATAA fd;
        ModLoaderFile mf;
        memset(&fd, 0, sizeof(fd));
        memset(&mf, 0, sizeof(mf));
        
        std::string filepath_buffer;
        filepath_buffer.reserve(100);
        
        MakeSureStringIsDirectory(dir, false);
        
        // Opens the search
        if((hSearch = FindFirstFileA((dir + mask).c_str(), &fd)) == INVALID_HANDLE_VALUE)
            return true;

        // Iterate on all files on this directory
        do
        {
            //  Ignore files beggining with '.' (including "." & "..")
            if(fd.cFileName[0] != '.' && fd.cFileName[0] != '*')
            {
                // Setup mf structure  
                mf.filename = (fd.cFileName);  // Just the filename
                mf.is_dir   = (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)!=0;  // Is it a directory?
                mf.recursion = bRecursive && mf.is_dir;                             // cb() can set this to false if it don't want to go recursive
                if(mf.filext = strrchr(mf.filename, '.'))
                    mf.filext = mf.filext + 1;
                else
                    mf.filext = "";
                
                // get dir+filename, if dir append '\\'
                filepath_buffer = dir + mf.filename;
                if(mf.is_dir) MakeSureStringIsDirectory(filepath_buffer);
                
                // Finish setupping mf structure
                mf.filepath = filepath_buffer.c_str();          // Filename and directory related to the first call to ForeachFile
                mf.filepath_len = filepath_buffer.length();     // Length, may be helpful for plugins analyzes
     
                // Call cb() and go recursive if asked to...
                if(!cb(mf)
                ||(mf.recursion && !ForeachFile(dir + mf.filename, mask, bRecursive, cb)))
                {
                    FindClose(hSearch);
                    return false;
                }
            }
        }
        while(FindNextFile(hSearch, &fd));   // Next...

        // Done
        FindClose(hSearch);  
        return true;
    }

    template<class T>
    inline bool ForeachFile(std::string mask, bool bRecursive, T cb)
    {
        return ForeachFile<T>("", mask, bRecursive, cb);
    }
    
    
    /*
     *      Checks if a file is inside an named folder, or just inside it
     *      @file File path to check
     *      @bJust  Returns true only if just right inside the folder
     *      @folder Folder path
     */
    inline bool IsFileInsideFolder(std::string file, bool bJust, std::string folder)
    {
        file = NormalizePath(file);
        folder= NormalizePath(folder);
        
        if(bJust)   // Only if just right inside the folder?
        {
            //
            size_t last = file.find_last_of(cNormalizedSlash);
            if(last != file.npos && last >= folder.length())
            {
                // Find pos to start the comparation and number chars to compare
                size_t pos = last - folder.length();
                size_t n  = last - pos;
                // Get result
                return (file.compare(pos, n, folder) == 0);
            }
            return false;
        }
        else
        {
            // Simple enougth, just find folder in file
            return (file.find(folder) != file.npos);
        }
    }
    
    
    /*
     *  GetLastPathComponent
     *      @path: Path to get the last component from
     *      @return: Returns the last path component position in the string
     */
    inline std::string::size_type GetLastPathComponent(std::string path)
    {
        // Remove any slash at the end of the string, so our finding can be safe
        while(path.back() == '/' || path.back() == '\\') path.pop_back();
        // Do the search
        size_t pos = path.find_last_of("/\\");
        return (pos == path.npos? 0 : pos + 1);
    }
    
    
    /*
     *  IsFileExtension
     *      @str: File
     *      @ext: Extension
     * 
     *      This is just a case-insensitive comparision between str and ext
     */
    inline bool IsFileExtension(const char* str, const char* ext)
    {
        return (!strcmp(str, ext, false));
    }

    
    /*
     * IsDirectoryA
     *      WinAPI-like function to check if a directory exists
     *      @szPath: Directory to check
     */
    inline BOOL IsDirectoryA(LPCTSTR szPath)
    {
      DWORD dwAttrib = GetFileAttributes(szPath);
      return (dwAttrib != INVALID_FILE_ATTRIBUTES && 
             (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
    }
    
    /*
     * IsFileA
     *      WinAPI-like function to check if a file or directory exists
     *      @szPath: Directory to check
     */
    inline BOOL IsPathA(LPCTSTR szPath)
    {
      DWORD dwAttrib = GetFileAttributesA(szPath);
      return (dwAttrib != INVALID_FILE_ATTRIBUTES);
    }

    inline BOOL IsPathW(LPCWSTR szPath)
    {
      DWORD dwAttrib = GetFileAttributesW(szPath);
      return (dwAttrib != INVALID_FILE_ATTRIBUTES);
    }
    
    template<class T> inline BOOL IsPath(const T* szPath)
    {
        return IsPathA(szPath);
    }
    
    template<> inline BOOL IsPath(const wchar_t* szPath)
    {
        return IsPathW(szPath);
    }
    
    
    
    /*
     * MakeSureDirectoryExistA
     *      WinAPI-like function to make sure a directory exists, if not, create it
     *      @szPath: Directory to check
     */
    inline BOOL MakeSureDirectoryExistA(LPCTSTR szPath)
    {
        if(!IsDirectoryA(szPath))
        {
            CreateDirectoryA(szPath, NULL);
            return FALSE;
        }
        return TRUE;
    }
    
    /*
     *  CopyDirectoryA
     *      WinAPI-like function that copies the full directory @szFrom to @szTo
     *      If @szTo doesn't exist, it is created
     */
    inline BOOL CopyDirectoryA(LPCTSTR szFrom, LPCTSTR szTo)
    {
        if(CreateDirectoryA(szTo, NULL))
        {
            ForeachFile(szFrom, "*.*", false, [&szFrom, &szTo](ModLoaderFile& file)
            {
                CHAR szToFile[MAX_PATH], szFromFile[MAX_PATH];
                const char* pPath = file.filename;
                
                sprintf(szToFile, "%s\\%s", szTo, pPath);
                sprintf(szFromFile, "%s\\%s", szFrom, pPath);

                if(file.is_dir) // Call myself again for recursion
                    CopyDirectoryA(szFromFile, szToFile);
                else
                    CopyFileA(szFromFile, szToFile, FALSE);

                return true;
            });
            
            return TRUE;
        }
        
        return FALSE;
    }
    
    /*
     *  DestroyDirectoryA
     *      WinAPI-like function that deletes the path @szPath fully
     */
    inline BOOL DestroyDirectoryA(LPCTSTR szPath)
    {
        ForeachFile(szPath, "*.*", false, [&szPath](ModLoaderFile& file)
        {
            CHAR szPathFile[MAX_PATH];
            const char* pPath = file.filename;
                
            sprintf(szPathFile, "%s\\%s", szPath, pPath);
                
            if(file.is_dir)
                DestroyDirectoryA(szPathFile);
            else
                DeleteFileA(szPathFile);

            return TRUE;
        });
        
        return RemoveDirectoryA(szPath);
    }
    
    /*
     *  GetFileSize
     *      WinAPI-like function that gets the file size of @szPath
     */
    inline LONGLONG GetFileSize(LPCTSTR szPath)
    {
        WIN32_FILE_ATTRIBUTE_DATA fad; LARGE_INTEGER size;

        if(!GetFileAttributesExA(szPath, GetFileExInfoStandard, &fad))
                return 0;

        size.HighPart = fad.nFileSizeHigh;
        size.LowPart = fad.nFileSizeLow;
        return size.QuadPart;
    }
    
    
    template<class T>
    static bool IsAbsolutePath(const T* str)
    {
        return (
                (str[0] == '\\' || str[0] == '/')
            ||  (isalpha(str[0]) && str[1] == ':' && (str[2] == '\\' || str[2] == '/'))
          );
    }
    
    
    /* RAII for SetCurrentDirectory */
    struct scoped_chdir
    {
        char buffer[MAX_PATH];
        
        scoped_chdir(const char* newDir)
        {
            GetCurrentDirectoryA(sizeof(buffer), buffer);
            SetCurrentDirectoryA(newDir);
        }
        
        ~scoped_chdir()
        {
            SetCurrentDirectoryA(buffer);
        }
    };
    
}
    
#endif	/* MODLOADER_UTIL_PATH_HPP */

