FROM debian@sha256:08db48d59c0a91afb802ebafc921be3154e200c452e4d0b19634b426b03e0e25 AS toolchain
RUN apt-get update && \
	apt-get install -y curl xz-utils make file cmake lld clang git patchelf gcc libgcc-s1
RUN mkdir /build
ADD docker/libddwaf/common/Makefile docker/libddwaf/common/CHECKSUMS /build/
ENV TARGET_ARCH=x86_64
RUN cd /build && make install && make clean

FROM toolchain AS package_build
ADD . /build/src/libddwaf
ADD docker/libddwaf/x86_64/Toolchain.cmake /build/
RUN make -C /build libddwaf && \
	make -C /build/build/libddwaf testPowerWAF && \
	mkdir -p /opt && \
	cp /build/build/libddwaf/*.tar.gz /opt && \
	cp /build/build/libddwaf/tests/testPowerWAF /build && \
	rm -rf /build/build
RUN tar -C /muslsysroot/lib -czf /opt/libc++-static-${TARGET_ARCH}-linux.tar.gz libc++.a libc++abi.a libunwind.a

# PowerWAF tetsts
RUN cd /build/src/libddwaf/tests && /build/testPowerWAF

# Smoke tests building
RUN tar -xvzf /opt/libddwaf-*.tar.gz -C /muslsysroot --strip-components=1
ENV VERBOSE=1
WORKDIR /build/src/libddwaf/docker/libddwaf/x86_64/smoketest
RUN ./smoke_musl_shared.sh
RUN ./smoke_musl_static_libgcc.sh
RUN ./smoke_musl_static_llvm.sh
RUN ./smoke_gnu_shared.sh
RUN ./smoke_gnu_static.sh
RUN find . -type f -name smoketest -exec echo '{}' \; -exec bash -c "readelf -d -l '{}' | grep 'NEEDED\|Requesting'" \;
# Copy smoke tests for running on Docker
RUN mkdir /opt/smoketest
RUN find . -type f -name smoketest -exec bash -c 'cp -v "{}" /opt/smoketest/$(echo "{}" | awk -F/ "{print \$(NF-1);}")-smoketest' \;

FROM scratch
COPY --from=package_build /opt /
