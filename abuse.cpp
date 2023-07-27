#include <jni.h>
#include <stdint.h>
#include <stdio.h>

static jclass abuseClass = NULL;

extern "C" jlong
Java_sample_DowncallAbuse_setupAndGetEnv(JNIEnv *env, jclass clazz)
{
	abuseClass = (jclass)env->NewGlobalRef(clazz);

	if ((NULL == abuseClass) || env->ExceptionCheck()) {
		fprintf(stderr, "Can't create global reference to class sample/DowncallAbuse.\n");
	}

	printf("setupAndGetEnv() -> %p.\n", env);

	return (jlong)(intptr_t)env;
}

extern "C" void
non_trivial(jlong env_)
{
	JNIEnv *env = (JNIEnv *)(intptr_t)env_;

	printf("Begin non_trivial(env=%p).\n", env);

	jmethodID upcall = env->GetStaticMethodID(abuseClass, "upcall", "(I)V");

	if ((NULL == upcall) || env->ExceptionCheck()) {
		fprintf(stderr, "Can't find method sample/DowncallAbuse.upcall(int).\n");
		return;
	}

	for (jint i = 0; i < 5; ++i) {
		env->CallStaticVoidMethod(abuseClass, upcall, i);

		if (env->ExceptionCheck()) {
			fprintf(stderr, "Call to method sample/DowncallAbuse.upcall(int) failed.\n");
			return;
		}
	}

	printf("End non_trivial().\n");
}
