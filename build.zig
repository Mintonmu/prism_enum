const std = @import("std");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const lib = b.addLibrary(.{
        .name = "prism_enum",
        .linkage = .static,
        .root_module = b.createModule(.{
            .target = target,
            .optimize = optimize,
            .link_libc = true,
            .link_libcpp = true,
        }),
    });

    lib.root_module.addCSourceFiles(.{
        .files = &.{
            "src/prism_enum.cpp",
            "src/prism_impl.cpp",
        },
        .flags = &.{
            "-std=c++20",
            "-Wall",
            "-Wextra",
            "-Wpedantic",
        },
    });

    lib.root_module.addIncludePath(b.path("include"));

    b.installArtifact(lib);
    lib.installHeadersDirectory(b.path("include"), "", .{});
}
