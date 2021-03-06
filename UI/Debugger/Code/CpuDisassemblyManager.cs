﻿using Mesen.GUI.Debugger.Controls;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Mesen.GUI.Debugger.Code
{
	public class CpuDisassemblyManager : IDisassemblyManager
	{
		private CodeDataProvider _provider;

		public ICodeDataProvider Provider { get { return this._provider; } }
		public int AddressSize { get { return 6; } }
		public int ByteCodeSize { get { return 4; } }

		public void RefreshCode()
		{
			this._provider = new CodeDataProvider(CpuType.Cpu);
		}

		public void ToggleBreakpoint(int lineIndex)
		{
			int address = this._provider.GetLineAddress(lineIndex);
			if(address >= 0) {
				BreakpointManager.ToggleBreakpoint(new AddressInfo() {
					Address = address,
					Type = SnesMemoryType.CpuMemory
				});
			}
		}

		public void EnableDisableBreakpoint(int lineIndex)
		{
			int address = this._provider.GetLineAddress(lineIndex);
			if(address >= 0) {
				BreakpointManager.EnableDisableBreakpoint(new AddressInfo() {
					Address = address,
					Type = SnesMemoryType.CpuMemory
				});
			}
		}
	}
}
