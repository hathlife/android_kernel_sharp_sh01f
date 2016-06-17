/*
 * Copyright (C) 2013 Sharp.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#ifndef __MIYABI_H__
#define __MIYABI_H__

/* Miyabi hook functions */
int miyabi_ptrace_access_check(struct task_struct *child, unsigned int mode);
int miyabi_ptrace_traceme(struct task_struct *parent);
int miyabi_sb_mount(char *dev_name, struct path *path, char *type, unsigned long flags, void *data);
int miyabi_bprm_set_creds(struct linux_binprm *bprm);
int miyabi_sb_umount(struct vfsmount *mnt, int flags);
int miyabi_sb_pivotroot(struct path *old_path, struct path *new_path);
int miyabi_path_symlink(struct path *dir, struct dentry *dentry, const char *old_name);
int miyabi_path_link(struct dentry *old_dentry, struct path *new_dir, struct dentry *new_dentry);
int miyabi_path_chmod(struct path* path, umode_t mode);
int miyabi_path_chroot(struct path *path);
int miyabi_task_fix_setuid(struct cred *new, const struct cred *old, int flags);
int miyabi_socket_setsockopt(struct socket *sock, int level, int optname);
int miyabi_dentry_open(struct file *file, const struct cred *cred);
#endif /* __MIYABI_H__ */

