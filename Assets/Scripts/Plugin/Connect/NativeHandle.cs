using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace SpaceZ
{
    [StructLayout(LayoutKind.Sequential)]
    public struct NativeHandle
    {
        public int id;
        public int generation;
        public override bool Equals(object obj)
        {
            if (obj is not NativeHandle handle)
                return false;
            return id == handle.id && generation == handle.generation;
        }
        public override int GetHashCode() => (id, generation).GetHashCode();
        public override string ToString() => $"({id},{generation})";

        public static NativeHandle NULL => new NativeHandle{id = -1, generation = 0};

        public static bool operator==(NativeHandle a, NativeHandle b)
        => a.id == b.id && a.generation == b.generation;
        public static bool operator!=(NativeHandle a, NativeHandle b)
        => !(a == b);
        public static bool operator<(NativeHandle a, NativeHandle b)
        {
            if(a.generation != b.generation)
                return a.generation < b.generation;
            return a.id < b.id;
        }
        public static bool operator>(NativeHandle a, NativeHandle b)
        {
            if(a.generation != b.generation)
                return a.generation > b.generation;
            return a.id > b.id;
        }
    }
}
