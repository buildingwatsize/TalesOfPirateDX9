Files that were cleaned up during the x64 migration session.
These were untracked temp files created by the AI assistant.

Unfortunately, they were deleted (rm) instead of moved before this folder was created.
The files cannot be recovered from git because they were never committed.

Lost files:
  - MIGRATION_PLAN_LangRec_to_RES_STRING.md  (migration plan for LangRec -> RES_STRING)
  - ToP_Build_Progress.md                     (build progress tracker)
  - summarize-2026-04-09-22-06-10.md          (session summary snapshot)
  - dll_exports.txt                           (DLL export dump, regenerate with: dumpbin /exports Game.exe)
  - exports.txt                               (DLL export dump duplicate)
  - x64_patch.log.err                         (build error log)
  - sources/mapping_report.txt                (x86->x64 mapping analysis)
  - sources/mapping_summary.txt               (x86->x64 mapping summary)
  - claude-transcript/summarize.md            (transcript summary)
  - Client/scripts/table/character_lvup.txt.100 (backup of character_lvup.txt)
  - Client/log/font_trace.log                 (runtime font debug log, regenerated on next run)
  - Client/log/model_trace.log                (runtime model debug log, regenerated on next run)
  - Client/log/render_diag.log                (runtime render debug log, regenerated on next run)

To regenerate export dumps:
  dumpbin /exports Client\system\Game.exe > cleaned-up\dll_exports.txt
  dumpbin /exports Client\system\MindPower3D_D8R.dll >> cleaned-up\dll_exports.txt

Debug logs are auto-generated when the client runs with tracing enabled.
