/* Frame object interface */

#ifndef Py_CPYTHON_FRAMEOBJECT_H
#  error "this header file must not be included directly"
#endif

/* These values are chosen so that the inline functions below all
 * compare f_state to zero.
 */
enum _framestate {
    FRAME_CREATED = -2,         // 创建
    FRAME_SUSPENDED = -1,       // 暂停
    FRAME_EXECUTING = 0,        // 执行中
    FRAME_RETURNED = 1,         // 返回
    FRAME_UNWINDING = 2,        // 解开？
    FRAME_RAISED = 3,           // 提高？
    FRAME_CLEARED = 4           // 清理完毕
};

typedef signed char PyFrameState;

typedef struct {
    int b_type;                 /* what kind of block this is */    // 块的类型
    int b_handler;              /* where to jump to find handler */ // 跳转到哪里找到处理程序
    int b_level;                /* value stack level to pop to */   // 弹出值栈的级别
} PyTryBlock;

// 栈帧
struct _frame {
    PyObject_VAR_HEAD
    struct _frame *f_back;      /* previous frame, or NULL 前一个栈帧 */
    PyCodeObject *f_code;       /* code segment */      // 代码段
    PyObject *f_builtins;       /* builtin symbol table (PyDictObject) 内置符号表，字典 */
    PyObject *f_globals;        /* global symbol table (PyDictObject) 全局符号表 字典 */
    PyObject *f_locals;         /* local symbol table (any mapping)  当前作用域 映射 */
    PyObject **f_valuestack;    /* points after the last local */   // 指向最后一个局部变量的后面
    PyObject *f_trace;          /* Trace function */    // 跟踪函数
    int f_stackdepth;           /* Depth of value stack */  // 值栈深度
    char f_trace_lines;         /* Emit per-line trace events? */ // 每行跟踪事件
    char f_trace_opcodes;       /* Emit per-opcode trace events? */ // 每个操作码跟踪事件

    /* Borrowed reference to a generator, or NULL */
    PyObject *f_gen;        // 生成器

    int f_lasti;                /* Last instruction if called */    // 最后一条指令
    int f_lineno;               /* Current line number. Only valid if non-zero */   // 当前行号
    int f_iblock;               /* index in f_blockstack */ // f_blockstack的索引
    PyFrameState f_state;       /* What state the frame is in 状态 */   // 栈帧的状态
    PyTryBlock f_blockstack[CO_MAXBLOCKS]; /* for try and loop blocks  */   // try 和 loop 块，长度为CO_MAXBLOCKS=20
    PyObject *f_localsplus[1];  /* locals+stack, dynamically sized */   // 局部变量和栈，动态地长度
};

// 状态判断
static inline int _PyFrame_IsRunnable(struct _frame *f) {
    return f->f_state < FRAME_EXECUTING;
}

static inline int _PyFrame_IsExecuting(struct _frame *f) {
    return f->f_state == FRAME_EXECUTING;
}

static inline int _PyFrameHasCompleted(struct _frame *f) {
    return f->f_state > FRAME_EXECUTING;
}

/* Standard object interface */

PyAPI_DATA(PyTypeObject) PyFrame_Type;

#define PyFrame_Check(op) Py_IS_TYPE(op, &PyFrame_Type)

// 创建栈帧
PyAPI_FUNC(PyFrameObject *) PyFrame_New(PyThreadState *, PyCodeObject *,
                                        PyObject *, PyObject *);

/* only internal use */
PyFrameObject*
_PyFrame_New_NoTrack(PyThreadState *, PyFrameConstructor *, PyObject *);


/* The rest of the interface is specific for frame objects */

/* Block management functions */

PyAPI_FUNC(void) PyFrame_BlockSetup(PyFrameObject *, int, int, int);
PyAPI_FUNC(PyTryBlock *) PyFrame_BlockPop(PyFrameObject *);

/* Conversions between "fast locals" and locals in dictionary */

PyAPI_FUNC(void) PyFrame_LocalsToFast(PyFrameObject *, int);

PyAPI_FUNC(int) PyFrame_FastToLocalsWithError(PyFrameObject *f);
PyAPI_FUNC(void) PyFrame_FastToLocals(PyFrameObject *);

PyAPI_FUNC(void) _PyFrame_DebugMallocStats(FILE *out);

PyAPI_FUNC(PyFrameObject *) PyFrame_GetBack(PyFrameObject *frame);
