using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace SpaceZ.Framework.RunTime
{
public abstract class RunTimeAttribute : Attribute
{
    public int Order { get; private set; }
    public RunTimeAttribute(int order = 0)
    {
        Order = order;
    }
}
[AttributeUsage(AttributeTargets.Method, Inherited = false, AllowMultiple = false)]
public class RunTimeStartAttribute : RunTimeAttribute
{
    public RunTimeStartAttribute(int order = 0) : base(order){}
}
[AttributeUsage(AttributeTargets.Method, Inherited = false, AllowMultiple = false)]
public class RunTimeEndAttribute : RunTimeAttribute
{
    public RunTimeEndAttribute(int order = 0) : base(order){}
}
}