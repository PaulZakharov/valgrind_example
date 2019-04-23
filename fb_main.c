
#include "pub_tool_basics.h"
#include "pub_tool_tooliface.h"
#include "pub_tool_libcassert.h"
#include "pub_tool_libcprint.h"
#include "pub_tool_debuginfo.h"
#include "pub_tool_libcbase.h"
#include "pub_tool_options.h"
#include "pub_tool_machine.h"     // VG_(fnptr_to_fnentry)

static Bool ic_process_cmd_line_option(const HChar* arg)
{
   return True;
}

static void ic_print_usage(void)
{  
   VG_(printf)(
"    (none)\n"
   );
}

static void ic_print_debug_usage(void)
{  
   VG_(printf)(
"    (none)\n"
   );
}

static ULong n_puts  = 0;
static _Bool fast_exec = 1;

static void add_n_put(int n)
{
   n_puts+=n;
}

static void ic_post_clo_init(void)
{
}

static void count_put(IRSB* sbOut, HWord gcnt) {
	IRExpr* arg1 = mkIRExpr_HWord( gcnt );
    IRExpr** argv = mkIRExprVec_1(arg1);
	IRDirty* di = unsafeIRDirty_0_N( 1, "add_n_put",
            VG_(fnptr_to_fnentry)( &add_n_put ),
             argv);
    addStmtToIRSB( sbOut, IRStmt_Dirty(di) );
}

static
IRSB* ic_instrument ( VgCallbackClosure* closure,
                      IRSB* sbIn, 
                      const VexGuestLayout* layout, 
                      const VexGuestExtents* vge,
                      const VexArchInfo* archinfo_host,
                      IRType gWordTy, IRType hWordTy )
{
   Int        i;
   IRSB*      sbOut;

   /* Set up SB */
   sbOut = deepCopyIRSBExceptStmts(sbIn);
	ppIRSB(sbIn);
   // Copy verbatim any IR preamble preceding the first IMark
   i = 0;
   while (i < sbIn->stmts_used && sbIn->stmts[i]->tag != Ist_IMark) {
      if (sbIn->stmts[i]->tag != Ist_NoOp)
          addStmtToIRSB( sbOut, sbIn->stmts[i] );
      i++;
   }

   HWord gcnt = 0;
   for (/*use current i*/; i < sbIn->stmts_used; i++) {
      IRStmt* st = sbIn->stmts[i];
      if (!st || st->tag == Ist_NoOp) continue;

      switch (st->tag) {
         case Ist_Put:
            /* Count PUT. */
			gcnt++;
			if (!fast_exec) count_put(sbOut, 1);
			break;
		case Ist_Exit:
            if (fast_exec) count_put(sbOut, gcnt);
            gcnt = 0;
            break;
		case Ist_IMark:
			VG_(umsg)("Instr\n");
			break;
         default:
            break;
      }

      addStmtToIRSB( sbOut, st );
   }

   if ((gcnt != 0) && fast_exec)
   {
   	count_put(sbOut, gcnt);
   }

   return sbOut;
}

static void ic_fini(Int exitcode)
{
   VG_(umsg)("\n");
   VG_(umsg)("Host register stores:  %'llu\n", n_puts);
   VG_(umsg)("\n");
   VG_(umsg)("Exit code:       %d\n", exitcode);
}

static void ic_pre_clo_init(void)
{
   VG_(details_name)            ("PUTcount");
   VG_(details_version)         (NULL);
   VG_(details_description)     ("Simple Valgrind tool which counts host register stores");
   VG_(details_copyright_author)(
      "Copyright (C) 2002-2015, and GNU GPL'd, by IamIntel.");
   VG_(details_bug_reports_to)  (VG_BUGS_TO);
   VG_(details_avg_translation_sizeB) ( 200 );

   VG_(basic_tool_funcs)          (ic_post_clo_init,
                                   ic_instrument,
                                   ic_fini);
   VG_(needs_command_line_options)(ic_process_cmd_line_option,
                                   ic_print_usage,
                                   ic_print_debug_usage);
}

VG_DETERMINE_INTERFACE_VERSION(ic_pre_clo_init)
