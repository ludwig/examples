module Main where 
 
import Word
import Control.Monad
import Control.Monad.State
 
import Foreign.C
import Foreign.Ptr
import Foreign.Marshal.Array
 
-- * Heterogenous lists
data HNil = HNil
data HCons a b = HCons a b
 
instance (Show a, Show b, HList b) => Show (HCons a b) where
    show (HCons a b) = show a ++ " .:. " ++ show b
{-
instance Show HNil where
    show HNil = "[]"
-}
class HList a
-- instance HList HNil
-- instance (HList b) => HList (HCons a b)
 
infixr 5 .:.
(.:.) :: a -> b -> HCons a b
a .:. b = HCons a b
 
 
{-
instance CreateWidgets HNil where
    createWidgets _ HNil = return ()
-}
 
data HTrue
data HFalse
 
class HBool x
instance HBool HTrue
instance HBool HFalse
 
data Proxy e
proxy :: Proxy e
proxy = undefined;
 
toProxy :: e -> Proxy e
toProxy _ = undefined
 
class HBool b => TypeEq x y b | x y -> b
proxyEq :: TypeEq t t' b => Proxy t -> Proxy t' -> b
proxyEq _ _ = undefined
 
-- * Some Basic Widgets
 
data QPushButton
    = QPushButton String
      deriving Show
 
-- * Some Layout Widgets
 
data (QWidgetList a) => QHBox a 
    = QHBox a
      deriving Show
 
data (QWidgetList a) => QVBox a 
    = QVBox a
      deriving Show
 
-- * Add Widgets to Widget Class
 
class QWidget a
instance QWidget QPushButton
instance QWidget (QHBox a)
instance QWidget (QVBox a)
 
data NoParent
instance QWidget NoParent
 
noParent = nullPtr :: Ptr NoParent
 
-- * Widget List constraint
 
class QWidgetList a
instance (QWidget a, QWidgetList b) => (QWidgetList (HCons a b))
instance QWidgetList QPushButton
instance (QWidgetList a) => QWidgetList (QHBox a)
instance (QWidgetList a) => QWidgetList (QVBox a)
instance QWidgetList NoParent
 
-- instance QWidgetList HNil
 
qwidgets :: (Show a, QWidgetList a) => a -> IO ()
qwidgets a = print $ a
 
 
class CreateWidgets a where
    createWidgets :: (QWidget p) => (Ptr p) -> a -> IO ()
 
instance (CreateWidget a, CreateWidgets b) => CreateWidgets (HCons a b) where
    createWidgets parent (HCons w ws) =
        do createWidget parent w
           createWidgets parent ws
 
instance (CreateWidgets QPushButton) where
    createWidgets parent (QPushButton str) =
        do qstr <- qstring str
           qpb  <- new
           (qpb->>qpushbutton) qstr (castPtr parent) 0
           showWidget qpb
           return ()
 
instance (QWidgetList a, CreateWidgets a) => (CreateWidgets (QHBox a)) where
    createWidgets parent (QHBox children) =
        do qhbox <- new
           (qhbox->>qhbox_QHBox) (castPtr parent) 0 0
           createWidgets qhbox children
           showWidget qhbox
           return ()
 
instance (QWidgetList a, CreateWidgets a) => (CreateWidgets (QVBox a)) where
    createWidgets parent (QVBox children) =
        do qvbox <- new
           (qvbox->>qvbox_QVBox) (castPtr parent) 0 0
           createWidgets qvbox children
           showWidget qvbox
           return ()
 
 
{-
instance (CreateWidget a, QWidget b) => CreateWidgets (HCons a b) where
    createWidgets parent (HCons a b) =
    do createWidget parent a
       createWidget parent b
-}
 
{-
createWidgets :: (QWidgetList a, QWidget p) => p -> a -> IO ()
createWidgets _ HNil = return ()
createWidgets parent (HCons a b) =
    do createWidget parent a
       createWidgets parent b
-}
 
class (QWidget w) => (CreateWidget w) where
    createWidget :: (QWidget p) => (Ptr p) -> w -> IO (Ptr w)
 
instance (CreateWidget QPushButton) where
    createWidget parent (QPushButton str) =
        do qstr <- qstring str
           qpb  <- new
           (qpb->>qpushbutton) qstr (castPtr parent) 0
           showWidget qpb
           return qpb
 
instance (QWidgetList a, CreateWidgets a) => (CreateWidget (QHBox a)) where
    createWidget parent (QHBox children) =
        do qhbox <- new
           (qhbox->>qhbox_QHBox) (castPtr parent) 0 0
           createWidgets qhbox children
           showWidget qhbox
           return qhbox
 
instance (QWidgetList a, CreateWidgets a) => (CreateWidget (QVBox a)) where
    createWidget parent (QVBox children) =
        do qvbox <- new
           (qvbox->>qvbox_QVBox) (castPtr parent) 0 0
           createWidgets qvbox children
           showWidget qvbox
           return qvbox
 
-- garbage collection ?
qstring :: String -> IO (Ptr QString)
qstring str =
    do cstr <- newCString str
       qstr <- new
       (qstr->>qstring_QString) cstr
       return qstr
 
-- * Other
 
data KApplication = KApplication
data KCmdLineArgs = KCmdLineArgs
data QString = QString
 
class New a where
    new :: IO (Ptr a)
 
instance New KApplication where
    new = hnew kapplicationSize
 
instance New QString where
    new = hnew qstringSize
 
instance New QPushButton where
    new = hnew qpushButtonSize
 
instance New (QHBox a) where
    new = hnew qhboxsize
 
instance New (QVBox a) where
    new = hnew qvboxsize
 
kapplicationSize = 204
qstringSize = 4
qpushButtonSize = 140
qhboxsize = 152
qvboxsize = 152
 
foreign import ccall "_ZN12KApplicationC1Ebb" kapplication_KApplication :: (Ptr KApplication) -> CInt -> CInt -> IO ()
foreign import ccall "_ZN12QApplication4execEv" qapplication_exec :: (Ptr KApplication) -> IO Int
foreign import ccall "_Znwj" cnew :: Word -> IO (Ptr ())
foreign import ccall "_ZN12KCmdLineArgs4initEiPPcPKcS3_S3_b" kCmdLineArgs_init :: Int -> Ptr CString -> CString -> CString -> CString -> Int -> IO ()
foreign import ccall "_ZN7QStringC1EPKc" qstring_QString :: (Ptr QString) -> CString -> IO ()
foreign import ccall "_ZN11QPushButtonC1ERK7QStringP7QWidgetPKc" qpushbutton :: (Ptr QPushButton) -> (Ptr QString) -> (Ptr ()) -> Int -> IO ()
foreign import ccall "_ZN12QApplication13setMainWidgetEP7QWidget" setMainWidget :: (Ptr KApplication) -> (Ptr ()) -> IO ()
foreign import ccall "_ZN7QWidget4showEv" qshow :: (Ptr ()) -> IO ()
foreign import ccall "_ZN7QObject7connectEPKS_PKcS1_S3_" cconnect :: (Ptr ()) -> CString -> (Ptr ()) -> CString -> IO ()
foreign import ccall "_ZN5QHBoxC1EP7QWidgetPKcj" qhbox_QHBox :: (Ptr (QHBox a)) -> Ptr () -> Int -> Int -> IO ()
foreign import ccall "_ZN5QVBoxC1EP7QWidgetPKcj" qvbox_QVBox :: (Ptr (QVBox a)) -> Ptr () -> Int -> Int -> IO ()
 
showWidget :: (QWidget w) => (Ptr w) -> IO ()
showWidget w = qshow (castPtr w)
 
hnew :: (New a) => Word -> IO (Ptr a)
hnew size = 
    do o <- cnew size
       return (castPtr o)
 
connect :: Ptr a -> String -> Ptr b -> String -> IO ()
connect a signal b slot =
    withCString ('2':signal) $ \sigStr ->
    withCString ('1':slot) $ \slotStr ->
        cconnect (castPtr a) sigStr (castPtr b) slotStr
 
infixl 0 ->>
c ->> m = m c
 
kapplication :: String -> String -> String -> IO (Ptr KApplication)
kapplication name description version = 
    do arg     <- newCString name
       cargv   <- newArray [ arg]
       appname <- newCString "appname"
       desc    <- newCString "desc"
       ver     <- newCString "ver"
       kCmdLineArgs_init 1 cargv appname desc ver 0
       kapp <- new :: IO (Ptr KApplication)
       (kapp->> kapplication_KApplication) 1 1
       return kapp
 
-- * My Application
 
layout =
    QVBox (    QHBox ((QPushButton "1") .:. (QPushButton "2") .:. (QPushButton "3"))
       .:. QHBox ((QPushButton "4") .:. (QPushButton "5") .:. (QPushButton "6"))
       .:. QHBox ((QPushButton "7") .:. (QPushButton "8") .:. (QPushButton "9"))
      ) 
 
main =
    do kapp <- kapplication "Calculator" "A Calculator in Haskell" "1.0"
       mainWidget <- createWidget noParent layout
       (kapp ->> setMainWidget) (castPtr mainWidget)
       (kapp ->> qapplication_exec)
