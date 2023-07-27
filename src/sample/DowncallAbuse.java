package sample;

import java.lang.foreign.FunctionDescriptor;
import java.lang.foreign.Linker;
import java.lang.foreign.SymbolLookup;
import java.lang.foreign.ValueLayout;
import java.lang.invoke.MethodHandle;

public class DowncallAbuse {

	static {
		System.loadLibrary("abuse");
	}

	private static native long setupAndGetEnv();

	private static void invoke(MethodHandle handle, long env) throws Throwable {
		handle.invokeExact(env);
	}

	public static void main(String[] args) {
		System.out.println("Starting ...");

		MethodHandle handle = Linker.nativeLinker() //
				.downcallHandle( //
						SymbolLookup.loaderLookup().find("non_trivial").get(), //
						FunctionDescriptor.ofVoid(ValueLayout.JAVA_LONG), //
						Linker.Option.isTrivial());

		long env = setupAndGetEnv();

		try {
			System.out.format("Calling non_trivial() env=0x%x%n", env);

			invoke(handle, env);
		} catch (Throwable e) {
			e.printStackTrace();
			return;
		}

		System.out.println("Done.");
	}

	private static void upcall(int n) {
		System.out.format("upcall(%d)%n", n);
	}

}
