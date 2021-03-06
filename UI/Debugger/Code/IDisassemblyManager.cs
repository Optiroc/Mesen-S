﻿using Mesen.GUI.Debugger.Controls;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Mesen.GUI.Debugger.Code
{
	public interface IDisassemblyManager
	{
		ICodeDataProvider Provider { get; }

		int AddressSize { get; }
		int ByteCodeSize { get; }

		void RefreshCode();
		void ToggleBreakpoint(int lineIndex);
		void EnableDisableBreakpoint(int lineIndex);
	}
}
