using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;

public class NativeLoader : IDisposable
{
    private class NativeLibrary : IDisposable
    {
        public IntPtr Handle { get; private set; }
        public string Path { get; private set; }
        public int UserCount { get; private set; }

        public NativeLibrary(string path)
        {
            Path = path;
            Handle = LoadLibrary(path);
            if (Handle == IntPtr.Zero)
                throw new Exception($"【NativeLoader】Failed to load library: {path}");
            UserCount = 1;
            loadedLibraries.Add(path, this);
        }
        public void Load()
        {
            if(Handle == IntPtr.Zero)
                throw new Exception("【NativeLoader】Library handle is invalid.");
            UserCount++;
        }
        public void Unload()
        {
            if (Handle == IntPtr.Zero)
                throw new Exception("【NativeLoader】Library handle is invalid.");
            UserCount--;
            if (UserCount <= 0)
                Dispose();
        }
        public void Dispose()
        {
            if (Handle == IntPtr.Zero)
                return;
            if (!FreeLibrary(Handle))
            {
                int err = Marshal.GetLastWin32Error();
                throw new Exception($"【NativeLoader】FreeLibrary failed, WinErr:{err}");
            }
            Handle = IntPtr.Zero;
            loadedLibraries.Remove(Path);
        }
    }
    [DllImport("kernel32", SetLastError = true, CharSet = CharSet.Unicode)]
    private static extern IntPtr LoadLibrary([MarshalAs(UnmanagedType.LPWStr)] string path);
    [DllImport("kernel32", SetLastError = true)]
    private static extern bool FreeLibrary(IntPtr handle);
    [DllImport("kernel32", SetLastError = true, CharSet = CharSet.Ansi)]
    private static extern IntPtr GetProcAddress(IntPtr handle, [MarshalAs(UnmanagedType.LPStr)] string procName);
    private static Dictionary<string, NativeLibrary> loadedLibraries = new();
    private IntPtr _handle = IntPtr.Zero;
    private string _path;
    public NativeLoader(string path)
    {
        _path = PluginUtility.GetDllPath(path);
        if (loadedLibraries.ContainsKey(_path))
        {
            var lib = loadedLibraries[_path];
            lib.Load();
            _handle = lib.Handle;
        }
        else
        {
            var lib = new NativeLibrary(_path);
            _handle = lib.Handle;
        }
    }
    public void GetFunction<T>(string functionName, out T functionPointer) where T : Delegate
    {
        if (_handle == IntPtr.Zero)
            throw new Exception("【NativeLoader】Library handle is invalid.");
        IntPtr procAddress = GetProcAddress(_handle, functionName);
        if (procAddress == IntPtr.Zero)
            throw new Exception($"【NativeLoader】Failed to get function address for {functionName}");
        functionPointer = Marshal.GetDelegateForFunctionPointer<T>(procAddress);
    }
    ~NativeLoader() => Dispose();
    public void Dispose()
    {
        if(_handle == IntPtr.Zero)
            throw new Exception("【NativeLoader】Library handle is invalid.");
        if(loadedLibraries.TryGetValue(_path, out var lib))
            lib.Unload();
        _handle = IntPtr.Zero;
    }
}