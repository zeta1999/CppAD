var list_across0 = [
'_contents.htm',
'_reference.htm',
'_index.htm',
'_search.htm',
'_external.htm'
];
var list_up0 = [
'cppad.htm',
'appendix.htm',
'deprecated.htm',
'atomic_two.htm'
];
var list_down3 = [
'install.htm',
'theory.htm',
'ad.htm',
'adfun.htm',
'preprocessor.htm',
'multi_thread.htm',
'utility.htm',
'ipopt_solve.htm',
'example.htm',
'speed.htm',
'appendix.htm'
];
var list_down2 = [
'faq.htm',
'directory.htm',
'glossary.htm',
'bib.htm',
'wish_list.htm',
'whats_new.htm',
'deprecated.htm',
'compare_c.htm',
'numeric_ad.htm',
'addon.htm',
'license.htm'
];
var list_down1 = [
'include_deprecated.htm',
'fundeprecated.htm',
'comparechange.htm',
'omp_max_thread.htm',
'tracknewdel.htm',
'omp_alloc.htm',
'memory_leak.htm',
'epsilon.htm',
'test_vector.htm',
'cppad_ipopt_nlp.htm',
'atomic_one.htm',
'atomic_two.htm',
'atomic_two_example.htm',
'chkpoint_one.htm',
'zdouble.htm',
'autotools.htm'
];
var list_down0 = [
'atomic_two_ctor.htm',
'atomic_two_option.htm',
'atomic_two_afun.htm',
'atomic_two_forward.htm',
'atomic_two_reverse.htm',
'atomic_two_for_sparse_jac.htm',
'atomic_two_rev_sparse_jac.htm',
'atomic_two_for_sparse_hes.htm',
'atomic_two_rev_sparse_hes.htm',
'atomic_two_clear.htm'
];
var list_current0 = [
'atomic_two.htm#Deprecated 2019-01-01',
'atomic_two.htm#Syntax',
'atomic_two.htm#See Also',
'atomic_two.htm#Purpose',
'atomic_two.htm#Purpose.Speed',
'atomic_two.htm#Purpose.Reduce Memory',
'atomic_two.htm#Virtual Functions',
'atomic_two.htm#Examples',
'atomic_two.htm#Contents'
];
function choose_across0(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_across0[index-1];
}
function choose_up0(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_up0[index-1];
}
function choose_down3(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_down3[index-1];
}
function choose_down2(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_down2[index-1];
}
function choose_down1(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_down1[index-1];
}
function choose_down0(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_down0[index-1];
}
function choose_current0(item)
{	var index          = item.selectedIndex;
	item.selectedIndex = 0;
	if(index > 0)
		document.location = list_current0[index-1];
}
